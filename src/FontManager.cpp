#include <Graphics/FontManager.hpp>

#include <Graphics/Texture.hpp>
#include <Math/Vec2.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <string>


std::vector<gfx::Font> gfx::FontManager::m_fonts;
FT_Library gfx::FontManager::m_lib;
gfx::FontId gfx::FontManager::m_fonts_counter;


bool gfx::FontManager::initialize()
{
	m_fonts_counter = 1;
	return FT_Init_FreeType(&m_lib);
}

bool gfx::FontManager::finalize()
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
	{
		delete m_fonts[i].texture;
		FT_Done_Face(m_fonts[i].face);
	}
    FT_Done_FreeType(m_lib);
    return true;
}


gfx::FontId gfx::FontManager::loadFromFile(std::string path, unsigned int size)
{
	FT_Face face;
	if (FT_New_Face(m_lib, path.c_str(), 0, &face))
		return 0;
	FT_Set_Pixel_Sizes(face, 0, size);
	Font font;
	font.id = m_fonts_counter;
	font.face = face;
	font.texture = new Texture();
	font.size = size;

	font.texture->create();
	unsigned char* data = new unsigned char[0];
	font.texture->loadFromBuffer(data, 0, size, Texture::PixelFormat::RGBA);
	delete[] data;

	m_fonts.push_back(font);
	return m_fonts_counter++;
}


void gfx::FontManager::setSize(FontId id, unsigned int size)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (id == m_fonts[i].id)
		{
			FT_Set_Pixel_Sizes(m_fonts[i].face, 0, size);
			m_fonts[i].size = size;
			return;
		}
}


gfx::Texture* gfx::FontManager::getTexture(FontId id)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == id) return m_fonts[i].texture;
	return nullptr;
}

gfx::Font::Character gfx::FontManager::getCharacter(FontId id, unsigned char character)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == id)
		{
			for (unsigned int j = 0; j < m_fonts[i].characters.size(); j++)
				if (m_fonts[i].characters[j].value == character)
					return m_fonts[i].characters[j];
			if (loadChar(id, character))
				return m_fonts[i].characters.back();
		}
	return Font::Character();
}

unsigned int gfx::FontManager::getSize(FontId id)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == id) return m_fonts[i].size;
	return 0;
}



bool gfx::FontManager::loadChar(FontId id, unsigned char character)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == id)
		{
			if (FT_Load_Char(m_fonts[i].face, character, FT_LOAD_RENDER)) return false;

			unsigned int right_pos = m_fonts[i].texture->getSize().x;
			unsigned int new_size = right_pos + m_fonts[i].face->glyph->bitmap.width;

			unsigned int pixels_count = m_fonts[i].face->glyph->bitmap.width*m_fonts[i].face->glyph->bitmap.rows;
			unsigned char* pixels_rgba = new unsigned char[pixels_count*4];
			for (unsigned int j = 0; j < pixels_count; j++)
			{
				unsigned int pix_j = j*4;
				pixels_rgba[pix_j    ] = // a = b = c = 255;
				pixels_rgba[pix_j + 1] = 
				pixels_rgba[pix_j + 2] = 255;
				pixels_rgba[pix_j + 3] = m_fonts[i].face->glyph->bitmap.buffer[j];
			}

			m_fonts[i].texture->resize(mth::Vec2(new_size, m_fonts[i].size));
			m_fonts[i].texture->loadSubTexture(
				pixels_rgba,
				mth::Vec2(right_pos, 0),
				m_fonts[i].face->glyph->bitmap.width,
				m_fonts[i].face->glyph->bitmap.rows
			);

			delete[] pixels_rgba;

			m_fonts[i].characters.emplace_back();
			m_fonts[i].characters.back().value = character;
			m_fonts[i].characters.back().bearingX = m_fonts[i].face->glyph->bitmap_left;
			m_fonts[i].characters.back().bearingY = m_fonts[i].face->glyph->bitmap_top;
			m_fonts[i].characters.back().width = m_fonts[i].face->glyph->bitmap.width;
			m_fonts[i].characters.back().height = m_fonts[i].face->glyph->bitmap.rows;
			m_fonts[i].characters.back().advance = m_fonts[i].face->glyph->advance.x  >> 6;
			m_fonts[i].characters.back().shift_into_tex = right_pos;

			return true;
		}
	return false;
}


gfx::Font::Character::Character():
	value(0),
	bearingX(0),
	bearingY(0),
	width(0),
	height(0),
	advance(0),
	shift_into_tex(0) {}