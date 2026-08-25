#ifndef TEXTURE_CLASS_HEADER
#define TEXTURE_CLASS_HEADER

#include <Engine/Graphics/GL/TextureHandle.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Core/Resource.hpp>
#include <Engine/Math/Vec2.hpp>

#include <initializer_list>
#include <string>
#include <vector>


namespace eng::gfx
{
	class TexturePool;

	class Texture : public core::Resource
	{
	public:
		enum Error
		{
			NO_ERROR = 0,
			FILE_NOT_FOUND,
		};

		Texture();
		~Texture();

		void create(gl::PixelFormat pixel_format);
		void remove();

		bool setSmooth(bool flag);
		void setFlip(bool flip_x, bool flip_y);

		bool loadFromFile(std::initializer_list<std::string> paths);
		bool loadFromFile(const std::string& path);

		bool loadFromBuffer(void* image_data, unsigned int width, unsigned int height);
		bool loadSubTexture(void* subimage_data, const mth::Vec2& position, unsigned int width, unsigned int height);

		bool resize(const mth::Vec2& new_size);

		bool isTransparent() const;
		bool getFlipX() const;
		bool getFlipY() const;
		mth::Vec2 getSize() const;
		gl::PixelFormat getPixelFormat() const;
		unsigned int getNativeHandle() const;

		void bind() const;

		int getLastError();

	protected:
		gl::TextureHandle m_texture_handle;
		gl::TexturePool& m_texture_pool;
	};
}

#endif //TEXTURE_CLASS_HEADER