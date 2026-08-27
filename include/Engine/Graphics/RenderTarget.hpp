#ifndef RENDER_TARGET_CLASS_HEADER
#define RENDER_TARGET_CLASS_HEADER

#include <Engine/Math/Vec2.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Drawable.hpp>

#include <memory>

namespace eng::gfx
{
	class RenderTarget
	{
	public:
		RenderTarget();
		RenderTarget(unsigned int buffers_count, gl::PixelFormat* formats);
		~RenderTarget() = default;

		void setViewport(int x, int y, int width, int height);

		virtual void clear(const Color& color);
		void draw(Drawable& drawable, const RenderStates& states);

		Texture* getTexture(unsigned int index);

	protected:
		std::unique_ptr<gl::FrameBuffer> m_frame_buffer;
		mth::Vec2 m_viewport_pos;
		mth::Vec2 m_viewport_size;
	};
}

#endif //RENDER_TARGET_CLASS_HEADER