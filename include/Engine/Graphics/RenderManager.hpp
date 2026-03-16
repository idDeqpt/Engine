#ifndef RENDER_MANAGER_STATIC_CLASS_HEADER
#define RENDER_MANAGER_STATIC_CLASS_HEADER

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Shape2D.hpp>
#include <Engine/Graphics/View.hpp>
#include <vector>

namespace eng::gfx
{
	class Drawable;
	class CanvasItem;
	class Shader;
	class RenderTarget;

	class RenderManager
	{
	public:
		struct RenderPass
		{
			Shader* shader;
			std::vector<Texture::PixelFormat> color_attachments;
		};

		static void initialize();
		static void finalize();

		static void addObject(CanvasItem* object);
		static void removeObject(CanvasItem* object);
		static bool setRenderPipeline2d(const std::vector<RenderPass>& new_pipeline);

		static void draw2d();
		static void draw3d();

		static void render();

	protected:
		static std::vector<Drawable*> s_objects2d, s_objects3d;
		static std::vector<RenderPass> s_pipeline2d, s_pipeline3d;
		static std::vector<RenderTarget*> s_framebuffers2d, s_framebuffers3d;
		static View s_quad_view;
		static Shape2D s_quad_shape;

		static void addObject(Drawable* object, std::vector<Drawable*> vector);
		static void removeObject(Drawable* object, std::vector<Drawable*> vector);
		static bool setRenderPipeline2d(const std::vector<RenderPass>& new_pipeline);
	};
}

#endif //RENDER_MANAGER_STATIC_CLASS_HEADER