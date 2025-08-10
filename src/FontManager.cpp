#include <Graphics/FontManager.hpp>

#include <Graphics/TextureManager.hpp>
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
		FT_Done_Face(m_fonts[i].face);
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
	font.tex_id = TextureManager::create();
	font.size = size;

	unsigned char* data = new unsigned char[0];
	TextureManager::loadFromBuffer(font.tex_id, data, TextureManager::TextureData(0, size, TextureManager::TextureData::RED));
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


gfx::TextureId gfx::FontManager::getTexture(FontId id)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == id) return m_fonts[i].tex_id;
	return 0;
}

gfx::Font::Character gfx::FontManager::getCharacter(FontId font_id, unsigned char character)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == font_id)
		{
			for (unsigned int j = 0; j < m_fonts[i].characters.size(); j++)
				if (m_fonts[i].characters[j].value == character)
					return m_fonts[i].characters[j];
			if (loadChar(font_id, character))
				return m_fonts[i].characters.back();
		}
	return Font::Character();
}



bool gfx::FontManager::loadChar(FontId font_id, unsigned char character)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == font_id)
		{
			if (FT_Load_Char(m_fonts[i].face, character, FT_LOAD_RENDER)) return false;

			unsigned int right_pos = TextureManager::getData(m_fonts[i].tex_id).width;
			unsigned int new_size = right_pos + m_fonts[i].face->glyph->bitmap.width;

			TextureManager::resize(m_fonts[i].tex_id, mth::Vec2(new_size, m_fonts[i].size));
			TextureManager::loadSubTexture(
				m_fonts[i].tex_id,
				m_fonts[i].face->glyph->bitmap.buffer,
				mth::Vec2(right_pos, 0),
				TextureManager::TextureData(
					m_fonts[i].face->glyph->bitmap.width,
					m_fonts[i].face->glyph->bitmap.rows,
					TextureManager::TextureData::RED
			));

			m_fonts[i].characters.emplace_back();
			m_fonts[i].characters.back().value = character;
			m_fonts[i].characters.back().width = m_fonts[i].face->glyph->bitmap.width;
			m_fonts[i].characters.back().height = m_fonts[i].face->glyph->bitmap.rows;
			m_fonts[i].characters.back().advance = m_fonts[i].face->glyph->advance.x  >> 6;
			m_fonts[i].characters.back().shift_into_tex = right_pos;

			return true;
		}
	return false;
}


gfx::Font::Character::Character() : value(0), width(0), height(0), advance(0), shift_into_tex(0) {}