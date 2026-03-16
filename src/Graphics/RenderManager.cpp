#include <Engine/Graphics/RenderManager.hpp>

#include <Engine/Graphics/RenderScene.hpp>
#include <glad/glad.h>
#include <vector>


namespace eng
{

std::vector<gfx::RenderScene*> gfx::RenderManager::s_scenes;


void gfx::RenderManager::initialize()
{
	finalize();

	gladLoadGL();

	s_scenes.push_back(new RenderScene());
}

void gfx::RenderManager::finalize()
{
	for (auto& scene: s_scenes)
		if (scene) delete scene;
	s_scenes.clear();
}


gfx::RenderScene& gfx::RenderManager::createScene()
{
	RenderScene* scene = new RenderScene();
	s_scenes.push_back(scene);
	return *scene;
}

void gfx::RenderManager::removeScene(RenderScene& scene)
{
	for (unsigned int i = 1; i < s_scenes.size(); i++) // 1 for main scene saving
		if (&scene == s_scenes[i])
		{
			delete s_scenes[i];
			s_scenes.erase(s_scenes.begin() + i);
			return;
		}
}


gfx::RenderScene* gfx::RenderManager::getMainScene()
{
	if (s_scenes.size()) return s_scenes[0];
	return nullptr;
}

} // namespace eng