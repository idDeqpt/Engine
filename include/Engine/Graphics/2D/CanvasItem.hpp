#ifndef CANVAS_ITEM_CLASS_HEADER
#define CANVAS_ITEM_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <vector>


namespace eng::gfx
{
	class RenderTarget;

	class CanvasItem : public Drawable, public core::Node2D
	{
	public:
		struct Vertex
		{
			mth::Vec2 position;
			mth::Vec2 tex_coord;
		};

		CanvasItem();
		~CanvasItem();

		void setLayer(unsigned int new_layer);
		void setColor(const Color& new_color);
		void setTexture(Texture& new_texture);
		void setPrimitiveType(const PrimitiveType& new_primitive_type);

		unsigned int getLayer() const;
		Texture* getTexture();

		bool loadData(CanvasItem::Vertex* vertices, unsigned int vertices_count);
		void unloadData();

		void draw(RenderTarget* target, RenderStates& states);

	protected:
		unsigned int m_vertices_count;
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_layer;
		PrimitiveType m_primitive_type;
		Color m_color;
		Texture* m_texture;
	};
}

#endif //CANVAS_ITEM_CLASS_HEADER