#include <Engine/Graphics/Text2D.hpp>

#include <Engine/Graphics/CanvasItem.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec2.hpp>

#include <string>
#include <iostream>


gfx::Text2D::Text2D() : m_font(nullptr), m_text_need_update(false), m_last_font_characters(0), CanvasItem()
{
	setPrimitiveType(PrimitiveType::TRIANGLES);
}


void gfx::Text2D::setFont(Font& new_font)
{
	m_font = &new_font;
	m_text_need_update = true;
}

void gfx::Text2D::setString(const std::string& new_text)
{
	m_text = new_text;
	m_text_need_update = true;
}


std::string gfx::Text2D::getString()
{
	return m_text;
}


void gfx::Text2D::draw(Window* window, RenderStates& states)
{
	this->updateString();
	CanvasItem::draw(window, states);
}


void gfx::Text2D::updateString()
{
	if (!m_font) return;

	unsigned int relevant_characters_count = m_font->getLoadedCharactersCount();
	if (m_last_font_characters != relevant_characters_count)
		m_text_need_update = true;

	if (!m_text_need_update) return;

	if (m_text.size() && m_font->getTexture())
	{
		Font::Character* characters = new Font::Character[m_text.size()];
		for (unsigned int i = 0; i < m_text.size(); i++)
			characters[i] = m_font->getCharacter(m_text[i]);

		unsigned int vertices_count = m_text.size()*6;
		CanvasItem::Vertex* total_vertices = new CanvasItem::Vertex[vertices_count];
		CanvasItem::Vertex symbol_vertices[4];

		Texture* font_tex = m_font->getTexture();
		mth::Vec2 tex_size = font_tex->getSize();

		float last_char_x = 0;
		for (unsigned int i = 0; i < m_text.size(); i++)
		{
			unsigned int vert_i = i*6;
			mth::Vec2 tex_lt = {
				float(characters[i].shift_into_tex)/tex_size.x,
				0
			};
			mth::Vec2 tex_rd = {
				float(characters[i].shift_into_tex + characters[i].width)/tex_size.x,
				float(characters[i].height)/tex_size.y
			};

			mth::Vec2 char_pos = {
				last_char_x + characters[i].bearingX,
				m_font->getSize() - float(characters[i].bearingY)
			};

			symbol_vertices[0] = {{char_pos.x,                       char_pos.y},                        {tex_lt.x, tex_lt.y}};
			symbol_vertices[1] = {{char_pos.x + characters[i].width, char_pos.y},                        {tex_rd.x, tex_lt.y}};
			symbol_vertices[2] = {{char_pos.x,                       char_pos.y + characters[i].height}, {tex_lt.x, tex_rd.y}};
			symbol_vertices[3] = {{char_pos.x + characters[i].width, char_pos.y + characters[i].height}, {tex_rd.x, tex_rd.y}};

			total_vertices[vert_i]     = symbol_vertices[0];
			total_vertices[vert_i + 1] = symbol_vertices[1];
			total_vertices[vert_i + 2] = symbol_vertices[2];
			total_vertices[vert_i + 3] = symbol_vertices[2];
			total_vertices[vert_i + 4] = symbol_vertices[1];
			total_vertices[vert_i + 5] = symbol_vertices[3];

			last_char_x += characters[i].advance;
		}
		loadData(total_vertices, vertices_count);
		setTexture(*font_tex);

		delete[] characters;
		delete[] total_vertices;
	}
	else
		unloadData();

	m_text_need_update = false;
}