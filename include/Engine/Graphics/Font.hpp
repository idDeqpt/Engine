#ifndef FONT_CLASS_HEADER
#define FONT_CLASS_HEADER

#include <Engine/Graphics/Texture.hpp>

#include <string>
#include <vector>
#include <map>


struct FT_LibraryRec_;
struct FT_FaceRec_;
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

namespace eng::gfx
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

		struct Frame
		{
			Texture texture;
			std::vector<Character> characters;
		};

		Font();
		~Font();

		void remove();

		bool loadFromFile(std::string path);

		Texture* getTexture(unsigned int size);
		Font::Character getCharacter(unsigned char character, unsigned int size);
		unsigned int getLoadedCharactersCount(unsigned int size);

	protected:
		FT_Face m_face;
		std::map<unsigned int, Frame> m_frames;

		void createFrame(unsigned int size);
		bool loadChar(unsigned char character, unsigned int size);

		static bool s_inited;
		static unsigned int s_fonts_count;
		static FT_Library s_lib;

		static void initialize();
		static void finalize();
	};
}

#endif //FONT_CLASS_HEADER