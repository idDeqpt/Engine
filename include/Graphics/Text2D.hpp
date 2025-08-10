#ifndef TEXT_2D_CLASS_HEADER
#define TEXT_2D_CLASS_HEADER

#include <Graphics/CanvasItem.hpp>
#include <Graphics/FontManager.hpp>
#include <string>

namespace gfx
{
	class Text2D : public CanvasItem
	{
	public:
		Text2D();

		void setFont(FontId new_font_id);
		void setString(const std::string& new_text);

		std::string getString();

		void draw(Window* window, RenderStates& states);

	protected:
		FontId m_font_id;
		std::string m_text;
		bool m_text_need_update;

		void updateString();
	};
}

#endif //TEXT_2D_CLASS_HEADER