#ifndef TEXTURE_CLASS_HEADER
#define TEXTURE_CLASS_HEADER

#include <Engine/Core/Resource.hpp>
#include <Engine/Math/Vec2.hpp>

#include <initializer_list>
#include <string>
#include <vector>


typedef unsigned int GLuint;

namespace eng::gfx
{
	class Texture : public core::Resource
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

		enum Error
		{
			NO_ERROR,
			FILE_NOT_FOUND,
		};

		Texture();
		~Texture();

		void create(PixelFormat pixel_format);
		void remove();

		bool setSmooth(bool flag);
		void setFlip(bool flip_x, bool flip_y);

		bool loadFromFile(std::initializer_list<std::string> paths);
		bool loadFromFile(const std::string& path);
		bool loadFromBuffer(void* image_data, unsigned int width, unsigned int height);
		bool loadSubTexture(void* subimage_data, const mth::Vec2& position, unsigned int width, unsigned int height);

		bool resize(const mth::Vec2& new_size);

		bool getFlipX();
		bool getFlipY();
		mth::Vec2 getSize();
		PixelFormat getPixelFormat();
		GLuint getNativeHandle();
		int getLastError();

		void bind()   const;
		void unbind() const;

	protected:
		GLuint m_native_handle;
		bool m_flip_x, m_flip_y;
		unsigned int m_width;
		unsigned int m_height;
		PixelFormat m_format;
		unsigned char* m_pixels;
	};
}

#endif //TEXTURE_CLASS_HEADER