#ifndef RENDER_CANVAS_CLASS_HEADER
#define RENDER_CANVAS_CLASS_HEADER

#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/RenderScene.hpp>

namespace eng::gfx
{
	class RenderTarget;

	class RenderCanvas : public RenderScene
	{
	public:
		RenderCanvas();
		~RenderCanvas() = default;

		void setActiveCamera(Camera2D& camera);
		Camera2D& getActiveCamera();

		void draw(RenderTarget& target) override;

	protected:
		Camera2D* m_active_camera;
		Camera2D m_default_camera;
	};
}

#endif //RENDER_CANVAS_CLASS_HEADER