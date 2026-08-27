#ifndef GRAPHICS_OPEN_GL_3_3_FRAME_BUFFER_CLASS_HEADER
#define GRAPHICS_OPEN_GL_3_3_FRAME_BUFFER_CLASS_HEADER

#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>

namespace eng::gfx
{
    class Color;
    class Texture;

namespace gl::OpenGL33
{
	class FrameBuffer : public gl::FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer() override;
		
		void create(unsigned int buffers_count, gl::PixelFormat* formats) override;
		void remove() override;

		void setViewportSize(unsigned int width, unsigned int height) override;

		void clear(const Color& color) override;

		Texture* getTexture(unsigned int index) override;

		void bind() override;

	protected:
		unsigned int m_handle;
		unsigned int m_depth_buffer_handle;
		unsigned int m_color_buffers_count;
		Texture* m_buffer_textures;
	};
}
}

#endif //GRAPHICS_OPEN_GL_3_3_FRAME_BUFFER_CLASS_HEADER