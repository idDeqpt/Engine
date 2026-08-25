#ifndef GRAPHICS_GL_TEXTURE_IMPL_CLASS_HEADER
#define GRAPHICS_GL_TEXTURE_IMPL_CLASS_HEADER

#include <Engine/Graphics/GL/PixelFormat.hpp>

namespace eng::gfx::gl
{
	class TextureImpl
	{
	public:
		TextureImpl() = default;
		virtual ~TextureImpl() = default;

		virtual void create(PixelFormat pixel_format) = 0;
		virtual void remove() = 0;

		virtual bool setSmooth(bool flag) = 0;
		virtual void setFlip(bool flip_x, bool flip_y) = 0;

		virtual bool loadFromBuffer(void* image_data, unsigned int width, unsigned int height) = 0;
		virtual bool loadSubTexture(void* subimage_data, int x, int y, unsigned int width, unsigned int height) = 0;

		virtual bool resize(unsigned int width, unsigned int y) = 0;

		virtual bool isTransparent() = 0;
		virtual bool getFlipX() = 0;
		virtual bool getFlipY() = 0;
		virtual unsigned int getWidth() = 0;
		virtual unsigned int getHeight() = 0;
		virtual PixelFormat getPixelFormat() = 0;

		virtual void bind() = 0;

	protected:
		bool m_flip_x{false};
		bool m_flip_y{false};
		unsigned int m_width{0};
		unsigned int m_height{0};
		PixelFormat  m_format{gl::PixelFormat::RED};
		unsigned char* m_pixels{nullptr};
	};
}

#endif //GRAPHICS_GL_TEXTURE_IMPL_CLASS_HEADER