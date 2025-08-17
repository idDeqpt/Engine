#ifndef TEXTURE_CLASS_HEADER
#define TEXTURE_CLASS_HEADER

#include <string>
#include <vector>
#include <Engine/Math/Vec2.hpp>

typedef unsigned int GLuint;

namespace gfx
{
	class Texture
	{
	public:
		enum PixelFormat
		{
			RED = 0,
			BLUE,
			GREEN,
			ALPHA,
			RGB,
			SRGB,
			RGB32F,
			RGBA,
			SRGBA,
			RGBA32F,
		};

		Texture();
		~Texture();

		void create();
		void remove();

		bool loadFromFile(std::string path, bool sRGB = false);
		bool loadFromBuffer(void* image_data, unsigned int width, unsigned int height, PixelFormat format);
		bool loadSubTexture(void* subimage_data, const mth::Vec2& position, unsigned int width, unsigned int height);

		bool resize(const mth::Vec2& new_size);

		mth::Vec2 getSize();
		PixelFormat getPixelFormat();


		static void bind(Texture* texture_ptr);

	protected:
		GLuint m_native_handle;
		unsigned int m_width;
		unsigned int m_height;
		PixelFormat m_format;
		unsigned char* m_pixels;
	};
}

#endif //TEXTURE_CLASS_HEADER