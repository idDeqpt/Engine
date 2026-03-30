#ifndef TEXT_2D_CLASS_HEADER
#define TEXT_2D_CLASS_HEADER

#include <Engine/Graphics/2D/CanvasItem.hpp>
#include <Engine/Graphics/Font.hpp>
#include <string>

namespace eng::gfx
{
	class Text2D : public CanvasItem
	{
	public:
		Text2D();

		void setFont(Font& new_font);
		void setString(const std::string& new_text);
		void setCharacterSize(unsigned int new_size);

		std::string getString();
		unsigned int getCharacterSize();

		void draw(RenderTarget* target, RenderStates& states);

	protected:
		Font* m_font;
		std::string m_text;
		bool m_text_need_update;
		unsigned int m_last_font_characters;
		unsigned int m_character_size;

		void updateString();
	};
}

#endif //TEXT_2D_CLASS_HEADER