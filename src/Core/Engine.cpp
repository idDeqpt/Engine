#include <Engine/Core/Engine.hpp>

#include <Engine/Core/Node.hpp>
#include <Engine/Core/ResourceManager.hpp>
#include <Engine/System/Window.hpp>
#include <Engine/System/EventManager.hpp>
#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/RenderManager.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Math/Vec4.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>


namespace eng
{

core::Engine::Engine(Node& root):
	m_root_node(&root)
{
	m_window = new sys::Window(900, 600, "LearnOpenGL");

	eng::gfx::RenderManager::initialize();
	eng::sys::EventManager::initialize(m_window->getHandler());
	eng::gfx::LightManager::initialize();
}

core::Engine::~Engine()
{
	eng::gfx::RenderManager::finalize();
	eng::gfx::LightManager::finalize();

	m_window->destroy();
}


void core::Engine::setup()
{
	std::string shaders_dir = "E:/Programming/Projects/C++/Engine/include/Engine/Graphics/shaders";

	eng::gfx::Shader* shader3d_deferred = eng::core::ResourceManager::load<eng::gfx::Shader>({
		shaders_dir + "/default3d.vert",
		shaders_dir + "/default3d-deferred.frag"
	}).second;
	std::cout << "Shader 3d-deferred error: " << shader3d_deferred->getLastError() << std::endl;
	if (shader3d_deferred->getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 3d-deferred error log: " << shader3d_deferred->getLastErrorLog() << std::endl;

	eng::gfx::Shader* shader2d_deferred_light = eng::core::ResourceManager::load<eng::gfx::Shader>({
		shaders_dir + "/default2d.vert",
		shaders_dir + "/default3d-deferred-light.frag"
	}).second;
	std::cout << "Shader 2d-deferred-light error: " << shader2d_deferred_light->getLastError() << std::endl;
	if (shader2d_deferred_light->getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 2d-deferred-light error log: " << shader2d_deferred_light->getLastErrorLog() << std::endl;

	eng::gfx::Shader* shader2d = eng::core::ResourceManager::load<eng::gfx::Shader>({
		shaders_dir + "/default2d.vert",
		shaders_dir + "/default2d.frag"
	}).second;
	std::cout << "Shader 2d error: " << shader2d->getLastError() << std::endl;
	if (shader2d->getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 2d error log: " << shader2d->getLastErrorLog() << std::endl;

	std::vector<eng::gfx::RenderScene::RenderPass> pipeline2d = {
		{
			shader2d,
			{eng::gfx::Texture::PixelFormat::RGBA},
			{},
			nullptr
		}
	};

	std::vector<eng::gfx::RenderScene::RenderPass> pipeline3d = {
		{
			shader3d_deferred,
			{
				eng::gfx::Texture::PixelFormat::RGBA32F, //position
				eng::gfx::Texture::PixelFormat::RGBA32F, //normal
				eng::gfx::Texture::PixelFormat::RGBA,    //albedo
				eng::gfx::Texture::PixelFormat::RGBA,    //met rog hei ao
				eng::gfx::Texture::PixelFormat::RGBA     //emission
			},
			{},
			nullptr
		},
		{
			shader2d_deferred_light,
			{},
			{
				"uPosition",
				"uNormal",
				"uAlbedo"
			},
			[&](eng::gfx::Shader* sh){
				eng::gfx::Camera3D& active_view = eng::gfx::Camera3D::getActive();
				eng::mth::Vec3 view_loc_pos = active_view.getPosition();
				eng::mth::Vec4 view_glob_pos = active_view.getGlobalTransform3D().value().getMatrix()*eng::mth::Vec4(view_loc_pos.x, view_loc_pos.y, view_loc_pos.z, 1);
				sh->setUniform3fv("uViewPos", 1, &view_glob_pos.x);

				eng::gfx::LightManager::DirectionalLight light = eng::gfx::LightManager::getDirectionalLight();
				bool use_directional_light = light.direction.x || light.direction.y || light.direction.z;
				sh->setUniform1i("uUseDirectionalLight", use_directional_light);
				if (use_directional_light)
				{
					sh->setUniform3fv("uDirectionalLight.direction", 1, &light.direction.x);
					sh->setUniform3fv("uDirectionalLight.color", 1, &light.color.x);
				}
			}
		}
	};

	eng::gfx::RenderManager::getMainScene()->setClearColor(eng::gfx::Color(120));
	eng::gfx::RenderManager::getMainScene()->setRenderPipeline2d(pipeline2d);
	eng::gfx::RenderManager::getMainScene()->setRenderPipeline3d(pipeline3d);

	m_root_node->setup();
}

void core::Engine::mainLoop()
{
	float delta_time = 0;
	while(m_window->isOpen())
	{
		float start_time = glfwGetTime();

		sys::EventManager::pull();

		m_root_node->update(delta_time);

		eng::gfx::RenderManager::getMainScene()->render(*m_window);
		m_window->display();

		delta_time = glfwGetTime() - start_time;
		std::cout << delta_time << std::endl;
	}
}

}