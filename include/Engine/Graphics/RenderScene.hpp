#ifndef RENDER_SCENE_CLASS_HEADER
#define RENDER_SCENE_CLASS_HEADER

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Shape2D.hpp>
#include <Engine/Graphics/View.hpp>

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
		};

		RenderScene();
		~RenderScene();

		void setClearColor(Color color);

		void addObject(CanvasItem& object);
		void removeObject(CanvasItem& object);
		bool setRenderPipeline2d(const std::vector<RenderPass>& new_pipeline);

		void addObject3d(Drawable& object);
		void removeObject3d(Drawable& object);
		bool setRenderPipeline3d(const std::vector<RenderPass>& new_pipeline);

		void draw2d(RenderTarget& target);
		void draw3d(RenderTarget& target);

		void render(RenderTarget& target);

	protected:
		std::vector<Drawable*>     m_objects2d,      m_objects3d;
		std::vector<RenderPass>    m_pipeline2d,     m_pipeline3d;
		std::vector<RenderTarget*> m_framebuffers2d, m_framebuffers3d;
		View    m_quad_view;
		Color m_clear_color;

		void addObject(   Drawable& object, std::vector<Drawable*>& vector);
		void removeObject(Drawable& object, std::vector<Drawable*>& vector);
		bool setRenderPipeline(const std::vector<RenderPass>& new_pipeline, std::vector<RenderPass>& old_pipeline, std::vector<RenderTarget*>& framebuffers);
	};
}

#endif //RENDER_MANAGER_STATIC_CLASS_HEADER