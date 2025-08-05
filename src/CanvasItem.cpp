#include <Graphics/CanvasItem.hpp>

#include <Graphics/Color.hpp>
#include <Graphics/Drawable.hpp>
#include <Math/Transformable2.hpp>

#include <glad/glad.h>


gfx::CanvasItem::CanvasItem() : Drawable(), mth::Transformable2()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

gfx::CanvasItem::~CanvasItem()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}


void gfx::CanvasItem::setColor(const Color& new_color)
{
	color = new_color;
}