#ifndef FONT_MANAGER_STATIC_CLASS_HEADER
#define FONT_MANAGER_STATIC_CLASS_HEADER

#include <vector>
#include <string>
#include <Graphics/TextureManager.hpp>

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
			TextureId id;
			int width;
			int height;
			int bearingX;
			int bearingY;
			int advance;
		};

		FontId id;
		FT_Face face;
		std::vector<Character> characters;
	};

	class FontManager
	{
	public:
		static bool initialize();
		static bool finalize();

		static FontId loadFromFile(std::string path, unsigned int size = 100);

		static void setSize(FontId id, unsigned int size);

		static Font::Character getCharacter(FontId font_id, unsigned char character);

	protected:
		static std::vector<Font> m_fonts;
		static FT_Library m_lib;
		static FontId m_fonts_counter;

		static bool loadChar(FontId font_id, unsigned char character);
	};
}

#endif //FONT_MANAGER_STATIC_CLASS_HEADER