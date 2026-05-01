#include <Engine/Core/Engine.hpp>

#include <Engine/Context.hpp>
#include <Engine/Core/Node.hpp>
#include <Engine/Core/Timer.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Core/TimeManager.hpp>
#include <Engine/Core/ResourceManager.hpp>
#include <Engine/Core/SignalBus.hpp>

#include <Engine/System/Window.hpp>
#include <Engine/System/EventManager.hpp>

#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/Graphics/RenderManager.hpp>

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


namespace eng
{

core::Engine::Engine(Node& root):
	m_root_node(&root),
	m_framerate(60)
{
	Logger::debug("Start app");
	m_window = new sys::Window(900, 600, "LearnOpenGL");

	Logger::debug("Start initialization");
	m_context.create<core::TimeManager>();
	m_context.create<core::SignalBus>();
	m_context.create<gfx::RenderManager>();
	m_context.create<sys::EventManager>();
	m_context.create<core::ResourceManager>();
	m_context.create<gfx::LightManager>();
	m_context.create<gfx::RenderScene>();
	m_context.create<phy::PhysicsWorld>();
	m_context.get<sys::EventManager>().setActiveWindow(*m_window);
	Logger::debug("End initialization");
}

core::Engine::~Engine()
{
	Logger::debug("Start finalization");
	m_context.shutdown();
	Logger::debug("End finalization");

	m_window->destroy();
	Logger::debug("End app");
}


void core::Engine::setup()
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
			{eng::gfx::Texture::PixelFormat::RGBA},
			{},
			nullptr,
			{0, 0},
			{900, 600}
		}
	};

	std::vector<eng::gfx::RenderScene::RenderPass> pipeline3d = {
		{
			&shader3d_deferred,
			{
				eng::gfx::Texture::PixelFormat::RGBA32F, //position
				eng::gfx::Texture::PixelFormat::RGBA32F, //normal
				eng::gfx::Texture::PixelFormat::RGBA,    //albedo
				eng::gfx::Texture::PixelFormat::RGBA,    //met rog hei ao
				eng::gfx::Texture::PixelFormat::RGBA     //emission
			},
			{},
			nullptr,
			{0, 0},
			{900, 600}
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
				gfx::Camera3D& active_view = m_context.get<gfx::RenderScene>().getActiveCamera3D();
				mth::Vec3 view_loc_pos = active_view.getPosition();
				mth::Vec4 view_glob_pos = active_view.getGlobalTransform3D().value().getMatrix()*mth::Vec4(view_loc_pos.x, view_loc_pos.y, view_loc_pos.z, 1);
				sh->setUniform3fv("uViewPos", 1, &view_glob_pos.x);

				gfx::LightManager::DirectionalLight light = m_context.get<gfx::LightManager>().getDirectionalLight();
				bool use_directional_light = light.direction.x || light.direction.y || light.direction.z;
				sh->setUniform1i("uUseDirectionalLight", use_directional_light);
				if (use_directional_light)
				{
					sh->setUniform3fv("uDirectionalLight.direction", 1, &light.direction.x);
					sh->setUniform3fv("uDirectionalLight.color", 1, &light.color.x);
				}
			},
			{0, 0},
			{900, 600}
		}
	};

	m_context.get<gfx::RenderScene>().setClearColor(gfx::Color(120));
	m_context.get<gfx::RenderScene>().setRenderPipeline2D(pipeline2d);
	m_context.get<gfx::RenderScene>().setRenderPipeline3D(pipeline3d);

	m_root_node->setName("root");
	m_root_node->setup(m_context);
	Logger::debug("End setup");
}

void core::Engine::mainLoop()
{
	sys::EventManager&   Event_Manager   = m_context.get<sys::EventManager>();
	gfx::RenderScene&    Render_Scene    = m_context.get<gfx::RenderScene>();
	phy::PhysicsWorld&   Physics_World   = m_context.get<phy::PhysicsWorld>();
	TimeManager&         Time_Manager    = m_context.get<TimeManager>();

	float real_frame_delta = 0;
	float full_frame_delta = 0;
	Timer timer;
	while(m_window->isOpen())
	{
		timer.restart();

		Event_Manager.pull();

		m_root_node->update(full_frame_delta);

		Physics_World.update(full_frame_delta);

		Render_Scene.render(*m_window);
		m_window->display();

		real_frame_delta = timer.getElapsedSeconds();
		float target_frame_delta = 1.0/m_framerate;
		if (real_frame_delta < target_frame_delta)
			Time_Manager.sleepSeconds(target_frame_delta - real_frame_delta);
		full_frame_delta = timer.getElapsedSeconds();
		if (full_frame_delta > 0.1) full_frame_delta = 0.1;
	}
}

}