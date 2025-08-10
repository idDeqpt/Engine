#ifndef TEXTURE_MANAGER_STATIC_CLASS_HEADER
#define TEXTURE_MANAGER_STATIC_CLASS_HEADER

#include <string>
#include <vector>
#include <Math/Vec2.hpp>

typedef unsigned int GLuint;

namespace gfx
{
	typedef GLuint TextureId;

	class TextureManager
	{
	public:
		struct TextureData
		{
			enum Channel
			{
				RED = 0,
				BLUE,
				GREEN,
				ALPHA,
				RGB,
				RGB32F,
				RGBA,
				RGBA32F,
			};

			TextureData();
			TextureData(int width, int height, Channel format);

			int width;
			int height;
			Channel format;
		};

		static void initialize();
		static void finalize();

		static bool bind(TextureId id);

		static TextureId create();
		static TextureId loadFromFile(std::string path);
		static bool loadFromBuffer(TextureId id, void* image_data, TextureData data);
		static bool loadSubTexture(TextureId id, void* subimage_data, const mth::Vec2& position, TextureData data);

		static bool resize(TextureId id, const mth::Vec2& new_size);
		static TextureData getData(TextureId id);

		static void deleteTexture(TextureId id);

	protected:
		static std::vector<TextureId> m_textures;
		static std::vector<TextureData> m_textures_data;
		static std::vector<unsigned char*> m_textures_pixels;
	};
}

#endif //TEXTURE_MANAGER_STATIC_CLASS_HEADER