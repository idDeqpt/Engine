#ifndef FONT_MANAGER_STATIC_CLASS_HEADER
#define FONT_MANAGER_STATIC_CLASS_HEADER

#include <vector>
#include <string>
#include <Graphics/Texture.hpp>

struct FT_LibraryRec_;
struct FT_FaceRec_;
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

namespace gfx
{
	typedef unsigned int FontId;

	struct Font
	{
		struct Character
		{
			unsigned char value;
			unsigned int bearingX;
			unsigned int bearingY;
			unsigned int width;
			unsigned int height;
			unsigned int advance;
			unsigned int shift_into_tex;

			Character();
		};

		FontId id;
		FT_Face face;
		Texture* texture;
		unsigned int size;
		std::vector<Character> characters;
	};

	class FontManager
	{
	public:
		static bool initialize();
		static bool finalize();

		static FontId loadFromFile(std::string path, unsigned int size = 100);

		static void setSize(FontId id, unsigned int size);

		static Texture* getTexture(FontId id);
		static Font::Character getCharacter(FontId id, unsigned char character);
		static unsigned int getSize(FontId id);

	protected:
		static std::vector<Font> m_fonts;
		static FT_Library m_lib;
		static FontId m_fonts_counter;

		static bool loadChar(FontId id, unsigned char character);
	};
}

#endif //FONT_MANAGER_STATIC_CLASS_HEADER