#include <Graphics/Shape.hpp>

#include <vector>
#include <Graphics/Drawable3.hpp>
#include <Math/Vec3.hpp>


gfx::Shape::Shape() : Drawable()
{
	m_points.clear();
	default_shader.loadFromBuffer
}


void gfx::Shape::addPoint(mth::Vec3 new_point)
{
	m_points.push_back(new_point);
}


void gfx::Shape::draw(Window* window) const
{

}



void gfx::Shape::update()
{
	
}