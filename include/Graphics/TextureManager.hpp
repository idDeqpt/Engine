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
				RGB4,
				RGB5,
				RGB8,
				RGB10,
				RGB12,
				RGB16,
				RGB16F,
				RGB32F,
				RGBA,
				RGBA2,
				RGBA4,
				RGBA8,
				RGBA12,
				RGBA16,
				RGBA16F,
				RGBA32F,
				RGB5_A1,
				RGB10_A2,
			};

			TextureData();
			TextureData(int width, int height, Channel internal_format, Channel format);

			int width;
			int height;
			Channel internal_format;
			Channel format;
		};

		static void initialize();
		static void finalize();

		static bool bind(TextureId id);

		static TextureId create();
		static TextureId loadFromFile(std::string path);
		static bool loadFromBuffer(TextureId id, unsigned char* image_data, TextureData data);
		static bool loadFromBuffer(TextureId id, float* image_data, TextureData data);

		static void deleteTexture(TextureId id);

	protected:
		static std::vector<TextureId> m_textures;
		static std::vector<TextureData> m_textures_data;
	};
}

#endif //TEXTURE_MANAGER_STATIC_CLASS_HEADER