#include <Graphics/FontManager.hpp>

#include <Graphics/TextureManager.hpp>

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

	m_fonts.push_back(font);
	return m_fonts_counter++;
}


void gfx::FontManager::setSize(FontId id, unsigned int size)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (id == m_fonts[i].id)
		{
			FT_Set_Pixel_Sizes(m_fonts[i].face, 0, size);
			return;
		}
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
	return {0, 0, 0, 0, 0, 0, 0};
}



bool gfx::FontManager::loadChar(FontId font_id, unsigned char character)
{
	for (unsigned int i = 0; i < m_fonts.size(); i++)
		if (m_fonts[i].id == font_id)
		{
			if (FT_Load_Char(m_fonts[i].face, character, FT_LOAD_RENDER)) return false;

			TextureId tex_id = TextureManager::create();
			TextureManager::loadFromBuffer(
				tex_id,
				(unsigned char*)m_fonts[i].face->glyph->bitmap.buffer,
				TextureManager::TextureData(
					m_fonts[i].face->glyph->bitmap.width,
					m_fonts[i].face->glyph->bitmap.rows,
					TextureManager::TextureData::RED,
					TextureManager::TextureData::RED
			));

			m_fonts[i].characters.push_back({
				character,
				tex_id,
				(int)m_fonts[i].face->glyph->bitmap.width,
				(int)m_fonts[i].face->glyph->bitmap.rows,
				(int)m_fonts[i].face->glyph->bitmap_left,
				(int)m_fonts[i].face->glyph->bitmap_top,
				(int)m_fonts[i].face->glyph->advance.x
			});

			return true;
		}
	return false;
}