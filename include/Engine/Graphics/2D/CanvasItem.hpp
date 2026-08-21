#ifndef CANVAS_ITEM_CLASS_HEADER
#define CANVAS_ITEM_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>
#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/RenderStates.hpp>

#include <vector>
#include <memory>


namespace eng::gfx
{
	class Texture;
	class RenderTarget;
	class RenderStates;

	class CanvasItem : public Drawable, public core::Node2D
	{
	public:
		struct Vertex
		{
			mth::Vec2 position;
			mth::Vec2 tex_coord;
		};

		CanvasItem();
		~CanvasItem() = default;

		void setLayer(unsigned int new_layer);
		void setColor(const Color& new_color);
		void setPrimitiveType(const PrimitiveType& new_primitive_type);
		virtual void setTexture(Texture* new_texture);

		unsigned int getLayer() const;
		Texture* getTexture();

		bool loadData(CanvasItem::Vertex* vertices, unsigned int vertices_count);
		void unloadData();

		virtual void draw(RenderTarget* target, const RenderStates& states) override;

	protected:
		std::unique_ptr<gl::ArrayBuffer> m_array_buffer;
		unsigned int m_vertices_count;
		unsigned int m_layer;
		PrimitiveType m_primitive_type;
		Color m_color;
		Texture* m_texture;
	};
}

#endif //CANVAS_ITEM_CLASS_HEADER