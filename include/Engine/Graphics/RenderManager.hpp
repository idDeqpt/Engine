#ifndef RENDER_MANAGER_STATIC_CLASS_HEADER
#define RENDER_MANAGER_STATIC_CLASS_HEADER

#include <vector>

namespace eng::gfx
{
	class RenderScene;

	class RenderManager
	{
	public:
		static void initialize();
		static void finalize();

		static RenderScene& createScene();
		static void removeScene(RenderScene& scene);

		static RenderScene* getMainScene();

	protected:
		static std::vector<RenderScene*> s_scenes;
	};
}

#endif //RENDER_MANAGER_STATIC_CLASS_HEADER