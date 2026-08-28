#ifndef RENDER_SCENE_CLASS_HEADER
#define RENDER_SCENE_CLASS_HEADER

#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/Color.hpp>

#include <functional>
#include <vector>

namespace eng::gfx
{
	class Drawable;
	class Shader;
	class RenderTarget;

	class RenderScene
	{
	public:
		struct RenderPass
		{
			Shader* shader;
			std::vector<gl::PixelFormat> color_attachments;
			std::vector<std::string>     input_buffers_names;
			std::function<void(Shader*)> uniforms_handler;
			mth::Vec2 viewport_position;
			mth::Vec2 viewport_size;
		};

		RenderScene();
		virtual ~RenderScene() = default;

		void addObject(Drawable& object);
		void removeObject(Drawable& object);

		bool setRenderPipeline(const std::vector<RenderPass>& new_pipeline);
		std::vector<RenderPass>& getRenderPipeline();

		virtual void draw(RenderTarget& target) = 0;

		void render(RenderTarget& target);

	protected:
		std::vector<Drawable*>     m_objects;
		std::vector<RenderPass>    m_pipeline;
		std::vector<RenderTarget*> m_framebuffers;
		Camera2D m_quad_view;
	};
}

#endif //RENDER_MANAGER_STATIC_CLASS_HEADER