#ifndef CANVAS_ITEM_CLASS_HEADER
#define CANVAS_ITEM_CLASS_HEADER

#include <Graphics/Color.hpp>
#include <Graphics/Drawable.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/TextureManager.hpp>
#include <Math/Transformable2.hpp>
#include <vector>


namespace gfx
{
	class Window;

	class CanvasItem : public Drawable, public mth::Transformable2
	{
	public:
		struct Vertex
		{
			mth::Vec2 position;
			mth::Vec2 tex_coord;
		};

		CanvasItem();
		~CanvasItem();

		void setColor(const Color& new_color);
		void setTexture(const TextureId& new_texture);

		bool loadData(CanvasItem::Vertex* vertices, unsigned int vertices_count);
		void unloadData();

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_vertices_count;
		unsigned int m_VAO;
		unsigned int m_VBO;
		Color m_color;
		bool m_visible;
		TextureId m_texture;
	};
}

#endif //CANVAS_ITEM_CLASS_HEADER