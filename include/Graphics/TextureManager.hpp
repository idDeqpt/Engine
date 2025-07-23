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
			int width;
			int height;
			int channels;
		};

		static void initialize();
		static void finalize();

		static bool bind(TextureId uid);

		static TextureId loadFromFile(std::string path);

	protected:
		static std::vector<TextureId> m_textures;
		static std::vector<TextureData> m_textures_data;
	};
}

#endif //TEXTURE_MANAGER_STATIC_CLASS_HEADER