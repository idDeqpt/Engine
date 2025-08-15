#ifndef FONT_CLASS_HEADER
#define FONT_CLASS_HEADER

#include <Engine/Graphics/Texture.hpp>
#include <vector>
#include <string>

struct FT_LibraryRec_;
struct FT_FaceRec_;
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

namespace gfx
{

	class Font
	{
	public:
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

		Font();
		~Font();

		void remove();

		bool loadFromFile(std::string path, unsigned int size);

		Texture* getTexture();
		Font::Character getCharacter(unsigned char character);
		unsigned int getLoadedCharactersCount();
		unsigned int getSize();

	protected:
		FT_Face m_face;
		Texture* m_texture;
		unsigned int m_size;
		std::vector<Character> m_characters;

		bool loadChar(unsigned char character);

		static bool s_inited;
		static unsigned int s_fonts_count;
		static FT_Library s_lib;

		static void initialize();
		static void finalize();
	};
}

#endif //FONT_CLASS_HEADER