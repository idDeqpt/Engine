#include <Engine/Core/Engine.hpp>

#include <Engine/Context.hpp>
#include <Engine/Core/Node.hpp>
#include <Engine/Core/Timer.hpp>
#include <Engine/Core/Delta.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Core/TimeManager.hpp>
#include <Engine/Core/ConfigManager.hpp>
#include <Engine/Core/PluginManager.hpp>
#include <Engine/Core/ResourceManager.hpp>
#include <Engine/Core/DefaultAppPlugin.hpp>

#include <Engine/System/Window.hpp>
#include <Engine/System/EventManager.hpp>

#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/Graphics/2D/RenderCanvas.hpp>
#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/3D/RenderWorld.hpp>
#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>

#include <Engine/Physics/PhysicsWorld.hpp>

#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Vec4.hpp>

#include "shaders/default2d-vert.hpp"
#include "shaders/default2d-frag.hpp"
#include "shaders/default3d-vert.hpp"
#include "shaders/default3d-deferred-frag.hpp"
#include "shaders/default3d-deferred-light-frag.hpp"

#include <iostream>
#include <string>


namespace eng::core
{

Engine::Engine(std::unique_ptr<Node> root)
{
	init();
	m_plugin_manager->addPlugin(std::make_unique<DefaultAppPlugin>(std::move(root)));
}

Engine::Engine()
{
	init();
}

Engine::~Engine()
{
	Logger::debug("Start finalization");
	m_root_node->destroy();
	m_root_node.reset();

	for (unsigned int i = 0; i < m_subscriptions.size(); i++)
		m_context.get<SignalBus>().unsubscribe(m_subscriptions[i]);
	m_context.shutdown();

	m_window.reset();
	gfx::gl::Api::getInstance()->finalize();
	sys::Window::finalize();

	Logger::debug("End finalization");
	Logger::debug("End app");
}


void Engine::setup()
{
	Logger::debug("Start setup");

	static gfx::Shader shader2d = gfx::Shader(shader::default2d_vert, shader::default2d_frag);
	Logger::debug(String("Default shader 2D loaded with error code: ") << shader2d.getLastError());
	if (shader2d.getLastError())
		Logger::error("Info: \n" + shader2d.getLastErrorLog());

	static gfx::Shader shader3d_deferred = gfx::Shader(shader::default3d_vert, shader::default3d_deferred_frag);
	Logger::debug(String("Default shader 3D deferred loaded with error code: ") << shader3d_deferred.getLastError());
	if (shader3d_deferred.getLastError())
		Logger::error("Info: \n" + shader3d_deferred.getLastErrorLog());

	static gfx::Shader shader2d_deferred_light = gfx::Shader(shader::default2d_vert, shader::default3d_deferred_light_frag);
	Logger::debug(String("Default shader 2D deferred light loaded with error code: ") << shader2d_deferred_light.getLastError());
	if (shader2d_deferred_light.getLastError())
		Logger::error("Info: \n" + shader2d_deferred_light.getLastErrorLog());


	std::vector<eng::gfx::RenderScene::RenderPass> pipeline2d = {
		{
			&shader2d,
			{eng::gfx::gl::PixelFormat::RGBA},
			{},
			nullptr,
			{0, 0},
			m_context.get<ConfigManager>().get<mth::Vec2>("window_viewport_size")
		}
	};

	std::vector<eng::gfx::RenderScene::RenderPass> pipeline3d = {
		{
			&shader3d_deferred,
			{
				eng::gfx::gl::PixelFormat::RGBA32F, //position
				eng::gfx::gl::PixelFormat::RGBA32F, //normal
				eng::gfx::gl::PixelFormat::RGBA,    //albedo
				eng::gfx::gl::PixelFormat::RGBA,    //met rog hei ao
				eng::gfx::gl::PixelFormat::RGBA     //emission
			},
			{},
			nullptr,
			{0, 0},
			m_context.get<ConfigManager>().get<mth::Vec2>("window_viewport_size")
		},
		{
			&shader2d_deferred_light,
			{},
			{
				"uPosition",
				"uNormal",
				"uAlbedo"
			},
			[&](gfx::Shader* sh){
				gfx::Camera3D& active_view = m_context.get<gfx::RenderWorld>().getActiveCamera();
				mth::Vec3 view_loc_pos = active_view.getPosition();
				mth::Vec4 view_glob_pos = active_view.getGlobalTransform().getMatrix()*mth::Vec4(view_loc_pos.x, view_loc_pos.y, view_loc_pos.z, 1);
				sh->setUniform3fv("uViewPos", &view_glob_pos.x);

				gfx::LightManager::DirectionalLight light = m_context.get<gfx::LightManager>().getDirectionalLight();
				bool use_directional_light = light.direction.x || light.direction.y || light.direction.z;
				sh->setUniform1i("uUseDirectionalLight", use_directional_light);
				if (use_directional_light)
				{
					sh->setUniform3fv("uDirectionalLight.direction", &light.direction.x);
					sh->setUniform3fv("uDirectionalLight.color", &light.color.x);
				}
			},
			{0, 0},
			m_context.get<ConfigManager>().get<mth::Vec2>("window_viewport_size")
		}
	};

	m_context.get<gfx::RenderWorld>().setRenderPipeline(pipeline3d);
	m_context.get<gfx::RenderCanvas>().setRenderPipeline(pipeline2d);

	m_root_node->setName("root");
	m_root_node->setup(m_context);

	m_plugin_manager->setup(*m_plugin_host);
	
	Logger::debug("End setup");
}

void Engine::mainLoop()
{
	sys::EventManager& Event_Manager = m_context.get<sys::EventManager>();
	gfx::RenderWorld&  Render_World  = m_context.get<gfx::RenderWorld>();
	gfx::RenderCanvas& Render_Canvas = m_context.get<gfx::RenderCanvas>();
	phy::PhysicsWorld& Physics_World = m_context.get<phy::PhysicsWorld>();
	TimeManager&       Time_Manager  = m_context.get<TimeManager>();

	Delta delta;
	delta.max = 0.1;
	float real_frame_delta = 0;
	float full_frame_delta = 0;
	Timer timer;
	while(m_window->isOpen())
	{
		timer.restart();

		Event_Manager.pull();

		m_root_node->update(delta);

		Physics_World.update(delta.frame);

		m_root_node->cleanupDestroyed();

		m_window->clear(gfx::Color(120));
		Render_World.render(*m_window);
		Render_Canvas.render(*m_window);
		m_window->display();

		delta.cpu = timer.getElapsedSeconds();
		const float target_frame_delta = 1.0/m_framerate;
		if (delta.cpu < target_frame_delta)
			Time_Manager.sleepSeconds(target_frame_delta - delta.cpu);
		delta.frame = timer.getElapsedSeconds();
		if (delta.frame > delta.max) delta.frame = delta.max;
	}
}


void Engine::init()
{
	Logger::debug("Start app");
	Logger::debug("Start initialization");
	
	m_framerate = 60;
	m_context.create<TimeManager>();

	gfx::gl::Api::createInstance();
	sys::Window::initialize();
	m_window = std::make_shared<sys::Window>(900, 600, "Engine");
	gfx::gl::Api::getInstance()->initialize();
	
	m_context.replace<sys::Window>(m_window);
	m_context.create<SignalBus>();
	m_context.create<ConfigManager>(m_context.get<SignalBus>());
	m_context.create<sys::EventManager>(m_context.get<SignalBus>());
	m_context.create<ResourceManager>();
	m_context.create<gfx::LightManager>();
	m_context.create<gfx::RenderWorld>();
	m_context.create<gfx::RenderCanvas>();
	m_context.create<phy::PhysicsWorld>();
	m_context.create<PluginManager>();
	m_context.get<sys::EventManager>().setActiveWindow(*m_window);

	SignalBus& sbus = m_context.get<SignalBus>();

	m_subscriptions.push_back(sbus.subscribe("on_change_config_window_title",
		[this](std::string title){
			m_window->setTitle(title);
	}));

	m_subscriptions.push_back(sbus.subscribe("on_change_config_window_size",
		[this](mth::Vec2 size){
			m_window->resize(size);
	}));

	m_subscriptions.push_back(sbus.subscribe("on_change_config_window_viewport_size",
		[this](mth::Vec2 size){
			m_window->setViewportSize(size);
			m_window->updateViewport();
	}));

	m_subscriptions.push_back(sbus.subscribe("on_change_config_window_viewport_centering",
		[this](mth::Vec2 ratio){
			m_window->setViewportCentering(ratio);
			m_window->updateViewport();
	}));

	m_subscriptions.push_back(sbus.subscribe("on_change_config_window_viewport_scaling",
		[this](std::string mode){
			if (mode == "fixed")
				m_window->setViewportScaling(sys::Window::ViewportScaling::FIXED);
			else if (mode == "stretch")
				m_window->setViewportScaling(sys::Window::ViewportScaling::STRETCH);
			m_window->updateViewport();
	}));

	m_context.get<ConfigManager>().notifyAll();

	m_root_node = std::make_unique<Node>();

	m_plugin_host = std::make_unique<PluginHost>(PluginHost{m_context, *m_root_node});
	m_plugin_manager = std::make_unique<PluginManager>();

	Logger::debug("End initialization");
}

} //namespace eng::core