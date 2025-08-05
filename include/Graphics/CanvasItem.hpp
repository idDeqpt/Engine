#ifndef CANVAS_ITEM_CLASS_HEADER
#define CANVAS_ITEM_CLASS_HEADER

#include <Graphics/Color.hpp>
#include <Graphics/Drawable.hpp>
#include <Graphics/RenderStates.hpp>
#include <Math/Transformable2.hpp>


namespace gfx
{
	class Window;

	class CanvasItem : public Drawable, public mth::Transformable2
	{
	public:
		CanvasItem();
		~CanvasItem();

		void setColor(const Color& new_color);

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_VAO;
		unsigned int m_VBO;
		Color color;
	};
}

#endif //CANVAS_ITEM_CLASS_HEADER