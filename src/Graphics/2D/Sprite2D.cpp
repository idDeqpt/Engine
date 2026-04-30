#include <Engine/Graphics/2D/Sprite2D.hpp>

#include <Engine/Graphics/2D/CanvasItem.hpp>
#include <Engine/Graphics/Texture.hpp>


namespace eng
{

gfx::Sprite2D::Sprite2D(): CanvasItem() {}


void gfx::Sprite2D::setTexture(Texture* new_texture)
{
	CanvasItem::setTexture(new_texture);

	CanvasItem::Vertex vertices[4] = {
		{{0,                      0},                      {0, 0}},
		{{m_texture->getSize().x, 0},                      {1, 0}},
		{{m_texture->getSize().x, m_texture->getSize().y}, {1, 1}},
		{{0,                      m_texture->getSize().y}, {0, 1}}
	};

	loadData(vertices, 4);
}

} //namespace eng