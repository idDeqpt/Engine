#include <Engine/Graphics/Font.hpp>

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec2.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
#include <map>


namespace eng
{

bool gfx::Font::s_inited = false;
unsigned int gfx::Font::s_fonts_count = 0;
FT_Library gfx::Font::s_lib;


void gfx::Font::initialize()
{
	if (!s_inited)
		FT_Init_FreeType(&s_lib);
}

void gfx::Font::finalize()
{
	if (!s_fonts_count)
		FT_Done_FreeType(s_lib);
}


gfx::Font::Font():
	m_face(nullptr)
{
	s_fonts_count++;
	Font::initialize();
}

gfx::Font::~Font()
{

	s_fonts_count--;
	Font::finalize();
}


void gfx::Font::remove()
{
	if (m_face != nullptr)
	{
		FT_Done_Face(m_face);
		m_face = nullptr;
	}
}


bool gfx::Font::loadFromFile(std::initializer_list<std::string> paths)
{
	return loadFromFile(*paths.begin());
}

bool gfx::Font::loadFromFile(const std::string& path)
{
	remove();

	FT_Face face;
	if (FT_New_Face(s_lib, path.c_str(), 0, &face))
		return false;
	m_face = face;

	return true;
}


gfx::Texture* gfx::Font::getTexture(unsigned int size)
{
	createFrame(size);
	return &(m_frames[size].texture);
}

gfx::Font::Character gfx::Font::getCharacter(unsigned char character, unsigned int size)
{
	createFrame(size);
	for (unsigned int i = 0; i < m_frames[size].characters.size(); i++)
		if (m_frames[size].characters[i].value == character)
			return m_frames[size].characters[i];

	if (loadChar(character, size))
		return m_frames[size].characters.back();

	return Font::Character();
}

unsigned int gfx::Font::getLoadedCharactersCount(unsigned int size)
{
	createFrame(size);
	return m_frames[size].characters.size();
}



void gfx::Font::createFrame(unsigned int size)
{
	auto it = m_frames.find(size);
	if (it != m_frames.end())
		return;

	m_frames[size].texture.create(Texture::PixelFormat::RGBA);
}


bool gfx::Font::loadChar(unsigned char character, unsigned int size)
{
	createFrame(size);
	FT_Set_Pixel_Sizes(m_face, 0, size);
	if (FT_Load_Char(m_face, character, FT_LOAD_RENDER)) return false;

	unsigned int right_pos = m_frames[size].texture.getSize().x;
	unsigned int new_size = right_pos + m_face->glyph->bitmap.width;

	unsigned int pixels_count = m_face->glyph->bitmap.width*m_face->glyph->bitmap.rows;
	unsigned char* pixels_rgba = new unsigned char[pixels_count*4];
	for (unsigned int i = 0; i < pixels_count; i++)
	{
		unsigned int pix_i = i*4;
		pixels_rgba[pix_i    ] = // a = b = c = 255;
		pixels_rgba[pix_i + 1] = 
		pixels_rgba[pix_i + 2] = 255;
		pixels_rgba[pix_i + 3] = m_face->glyph->bitmap.buffer[i];
	}

	m_frames[size].texture.resize(mth::Vec2(new_size, size));
	m_frames[size].texture.loadSubTexture(
		pixels_rgba,
		mth::Vec2(right_pos, 0),
		m_face->glyph->bitmap.width,
		m_face->glyph->bitmap.rows
	);

	delete[] pixels_rgba;

	m_frames[size].characters.emplace_back();
	m_frames[size].characters.back().value          = character;
	m_frames[size].characters.back().bearingX       = m_face->glyph->bitmap_left;
	m_frames[size].characters.back().bearingY       = m_face->glyph->bitmap_top;
	m_frames[size].characters.back().width          = m_face->glyph->bitmap.width;
	m_frames[size].characters.back().height         = m_face->glyph->bitmap.rows;
	m_frames[size].characters.back().advance        = m_face->glyph->advance.x  >> 6;
	m_frames[size].characters.back().shift_into_tex = right_pos;

	return true;
}


gfx::Font::Character::Character():
	value(0),
	bearingX(0),
	bearingY(0),
	width(0),
	height(0),
	advance(0),
	shift_into_tex(0) {}

} //namespace eng