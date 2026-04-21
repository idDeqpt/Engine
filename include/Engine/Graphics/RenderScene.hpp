#ifndef RENDER_SCENE_CLASS_HEADER
#define RENDER_SCENE_CLASS_HEADER

#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Color.hpp>

#include <functional>
#include <vector>

namespace eng::gfx
{
	class Drawable;
	class CanvasItem;
	class Shader;
	class RenderTarget;

	class RenderScene
	{
	public:
		struct RenderPass
		{
			Shader* shader;
			std::vector<Texture::PixelFormat> color_attachments;
			std::vector<std::string>          input_buffers_names;
			std::function<void(Shader*)>      uniforms_handler;
			mth::Vec2 viewport_position;
			mth::Vec2 viewport_size;
		};

		RenderScene();
		~RenderScene();

		void addObject(CanvasItem& object);
		void addObject3D(Drawable& object);

		void setClearColor(Color color);

		void removeObject(CanvasItem& object);
		void removeObject3D(Drawable& object);

		void setActiveCamera(Camera2D& camera);
		void setActiveCamera(Camera3D& camera);

		bool setRenderPipeline2D(const std::vector<RenderPass>& new_pipeline);
		bool setRenderPipeline3D(const std::vector<RenderPass>& new_pipeline);

		Camera2D& getActiveCamera2D();
		Camera3D& getActiveCamera3D();

		std::vector<RenderPass>& getRenderPipeline2D();
		std::vector<RenderPass>& getRenderPipeline3D();

		void draw2d(RenderTarget& target);
		void draw3d(RenderTarget& target);

		void render(RenderTarget& target);

	protected:
		std::vector<Drawable*>     m_objects2d,      m_objects3d;
		std::vector<RenderPass>    m_pipeline2d,     m_pipeline3d;
		std::vector<RenderTarget*> m_framebuffers2d, m_framebuffers3d;
		Camera2D* m_active_camera_2d;
		Camera3D* m_active_camera_3d;
		Camera2D m_default_camera_2d;
		Camera3D m_default_camera_3d;
		Camera2D m_quad_view;
		Color    m_clear_color;

		void addObject(   Drawable& object, std::vector<Drawable*>& vector);
		void removeObject(Drawable& object, std::vector<Drawable*>& vector);
		bool setRenderPipeline(const std::vector<RenderPass>& new_pipeline, std::vector<RenderPass>& old_pipeline, std::vector<RenderTarget*>& framebuffers);
	};
}

#endif //RENDER_MANAGER_STATIC_CLASS_HEADER