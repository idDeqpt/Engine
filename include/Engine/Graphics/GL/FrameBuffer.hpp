#ifndef GRAPHICS_GL_FRAME_BUFFER_CLASS_HEADER
#define GRAPHICS_GL_FRAME_BUFFER_CLASS_HEADER

namespace eng::gfx
{
    class Color;
    class Texture;

namespace gl
{
    enum class PixelFormat;
    
	class FrameBuffer
	{
	public:
		FrameBuffer() = default;
		virtual ~FrameBuffer() = default;
		
		virtual void create(unsigned int buffers_count, gl::PixelFormat* formats) = 0;
		virtual void remove() = 0;

		virtual void setViewportSize(unsigned int width, unsigned int height) = 0;

		virtual void clear(const Color& color) = 0;

		virtual Texture* getTexture(unsigned int index) = 0;
		
		virtual void bind() = 0;
	};
}
}

#endif //GRAPHICS_GL_FRAME_BUFFER_CLASS_HEADER