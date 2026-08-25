#ifndef GRAPHICS_OPEN_GL_3_3_TEXTURE_IMPL_CLASS_HEADER
#define GRAPHICS_OPEN_GL_3_3_TEXTURE_IMPL_CLASS_HEADER

#include <Engine/Graphics/GL/TextureImpl.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>

namespace eng::gfx::gl::Null
{
	class TextureImpl : public gl::TextureImpl
	{
	public:
		TextureImpl() = default;
		~TextureImpl() = default;

		void create(PixelFormat pixel_format) override;
		void remove() override;

		bool setSmooth(bool flag) override;
		void setFlip(bool flip_x, bool flip_y) override;

		bool loadFromBuffer(void* image_data, unsigned int width, unsigned int height) override;
		bool loadSubTexture(void* subimage_data, int x, int y, unsigned int width, unsigned int height) override;

		bool resize(unsigned int width, unsigned int y) override;

		bool isTransparent() override;
		bool getFlipX() override;
		bool getFlipY() override;
		unsigned int getWidth() override;
		unsigned int getHeight() override;
		PixelFormat getPixelFormat() override;

		void bind() override;
	};
}

#endif //GRAPHICS_OPEN_GL_3_3_TEXTURE_IMPL_CLASS_HEADER