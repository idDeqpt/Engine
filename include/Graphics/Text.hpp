#ifndef TEXT_CLASS_HEADER
#define TEXT_CLASS_HEADER

#include <Graphics/FontManager.hpp>
#include <Graphics/Drawable.hpp>
#include <Graphics/RenderStates.hpp>
#include <Math/Transformable3.hpp>
#include <string>


namespace gfx
{
	class Window;

	class Text : public Drawable, public mth::Transformable3
	{
	public:
		Text();
		~Text();

		void setFontId(FontId font_id);
		void setString(std::string str);

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_VAO;
		unsigned int m_VBO;
		FontId font_id;
		std::string str;
	};
}

#endif //TEXT_CLASS_HEADER