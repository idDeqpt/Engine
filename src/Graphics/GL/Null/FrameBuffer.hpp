#ifndef GRAPHICS_NULL_FRAME_BUFFER_CLASS_HEADER
#define GRAPHICS_NULL_FRAME_BUFFER_CLASS_HEADER

#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>

namespace eng::gfx
{
    class Color;
    class Texture;

namespace gl::Null
{
	class FrameBuffer : public gl::FrameBuffer
	{
	public:
		FrameBuffer() = default;
		~FrameBuffer() = default;
		
		void create(unsigned int buffers_count, gl::PixelFormat* formats) override;
		void remove() override;

		void setViewportSize(unsigned int width, unsigned int height) override;

		void clear(const Color& color) override;

		Texture* getTexture(unsigned int index) override;

		void bind() override;
	};
}
}

#endif //GRAPHICS_NULL_FRAME_BUFFER_CLASS_HEADER