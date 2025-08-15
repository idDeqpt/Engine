#include <Engine/Graphics/Font.hpp>

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec2.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <string>


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
	m_face(nullptr),
	m_texture(nullptr),
	m_size(0)
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
	if (m_texture != nullptr) {delete m_texture;     m_texture = nullptr;}
	if (m_face    != nullptr) {FT_Done_Face(m_face); m_face    = nullptr;}
}


bool gfx::Font::loadFromFile(std::string path, unsigned int size)
{
	remove();

	FT_Face face;
	if (FT_New_Face(s_lib, path.c_str(), 0, &face))
		return false;

	FT_Set_Pixel_Sizes(face, 0, size);

	m_face = face;
	m_texture = new Texture();
	m_size = size;

	m_texture->create();
	unsigned char* data = new unsigned char[0];
	m_texture->loadFromBuffer(data, 0, size, Texture::PixelFormat::RGBA);
	delete[] data;

	return true;
}


gfx::Texture* gfx::Font::getTexture()
{
	return m_texture;
}

gfx::Font::Character gfx::Font::getCharacter(unsigned char character)
{
	for (unsigned int i = 0; i < m_characters.size(); i++)
		if (m_characters[i].value == character)
			return m_characters[i];

	if (loadChar(character))
		return m_characters.back();

	return Font::Character();
}

unsigned int gfx::Font::getLoadedCharactersCount()
{
	return m_characters.size();
}

unsigned int gfx::Font::getSize()
{
	return m_size;
}



bool gfx::Font::loadChar(unsigned char character)
{
	if (FT_Load_Char(m_face, character, FT_LOAD_RENDER)) return false;

	unsigned int right_pos = m_texture->getSize().x;
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

	m_texture->resize(mth::Vec2(new_size, m_size));
	m_texture->loadSubTexture(
		pixels_rgba,
		mth::Vec2(right_pos, 0),
		m_face->glyph->bitmap.width,
		m_face->glyph->bitmap.rows
	);

	delete[] pixels_rgba;

	m_characters.emplace_back();
	m_characters.back().value          = character;
	m_characters.back().bearingX       = m_face->glyph->bitmap_left;
	m_characters.back().bearingY       = m_face->glyph->bitmap_top;
	m_characters.back().width          = m_face->glyph->bitmap.width;
	m_characters.back().height         = m_face->glyph->bitmap.rows;
	m_characters.back().advance        = m_face->glyph->advance.x  >> 6;
	m_characters.back().shift_into_tex = right_pos;

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