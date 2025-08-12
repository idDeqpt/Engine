#ifndef TEXT_2D_CLASS_HEADER
#define TEXT_2D_CLASS_HEADER

#include <Graphics/CanvasItem.hpp>
#include <Graphics/Font.hpp>
#include <string>

namespace gfx
{
	class Text2D : public CanvasItem
	{
	public:
		Text2D();

		void setFont(Font& new_font);
		void setString(const std::string& new_text);

		std::string getString();

		void draw(Window* window, RenderStates& states);

	protected:
		Font* m_font;
		std::string m_text;
		bool m_text_need_update;

		void updateString();
	};
}

#endif //TEXT_2D_CLASS_HEADER