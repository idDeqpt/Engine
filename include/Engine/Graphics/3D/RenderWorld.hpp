#ifndef RENDER_WORLD_CLASS_HEADER
#define RENDER_WORLD_CLASS_HEADER

#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/RenderScene.hpp>

namespace eng::gfx
{
	class RenderTarget;

	class RenderWorld : public RenderScene
	{
	public:
		RenderWorld();
		~RenderWorld() = default;

		void setActiveCamera(Camera3D& camera);
		Camera3D& getActiveCamera();

		void draw(RenderTarget& target) override;

	protected:
		Camera3D* m_active_camera;
		Camera3D m_default_camera;
	};
}

#endif //RENDER_WORLD_CLASS_HEADER