#include <Engine/Graphics/Shape2D.hpp>

#include <Engine/Math/Vec2.hpp>

#include <cmath>


void (*gfx::Shape2D::generators[])(gfx::Shape2D*) = {
	rectangleGenerator,
	circleGenerator
};


gfx::Shape2D::Shape2D():
	m_accuracy(30),
	m_size(1),
	m_need_update_points(true), CanvasItem()
{
	setType(Type::RECTANGLE);
}

gfx::Shape2D::Shape2D(Type type):
	m_accuracy(30),
	m_size(1),
	m_need_update_points(true), CanvasItem()
{
	setType(type);
}


void gfx::Shape2D::setType(const Type new_type)
{
	m_type = new_type;
	m_need_update_points = true;
	if (new_type == Type::CUSTOM)
		return;
	m_generator = Shape2D::generators[int(m_type)];
}

void gfx::Shape2D::setAccuracy(unsigned int accuracy)
{
	m_accuracy = accuracy;
	m_need_update_points = true;
}

void gfx::Shape2D::setSize(const mth::Vec2& new_size)
{
	m_size = new_size;
	m_need_update_points = true;
}

void gfx::Shape2D::setGenerator(void (*generator)(Shape2D*))
{
	m_type = Type::CUSTOM;
	m_generator = generator;
	m_need_update_points = true;
}


void gfx::Shape2D::draw(Window* window, RenderStates& states)
{
	updateShape();
	CanvasItem::draw(window, states);
}



void gfx::Shape2D::updateShape()
{
	if (m_need_update_points)
	{
		m_generator(this);
		m_need_update_points = false;
	}
}


void gfx::Shape2D::rectangleGenerator(Shape2D* shape)
{
	CanvasItem::Vertex vertices[4] = {
		{{0,               0},               {0, 0}},
		{{shape->m_size.x, 0},               {1, 0}},
		{{shape->m_size.x, shape->m_size.y}, {1, 1}},
		{{0,               shape->m_size.y}, {0, 1}}
	};
	shape->loadData(vertices, 4);
}

void gfx::Shape2D::circleGenerator(Shape2D* shape)
{
	const unsigned int n = shape->m_accuracy;
	CanvasItem::Vertex* vertices = new CanvasItem::Vertex[n];

	for (unsigned int i = 0; i < n; i++)
	{
		float points_ratio = (float(i)/n)*(3.1415926*2);
		mth::Vec2 point_on_circle = {
			(sin(points_ratio) + 1)*0.5f,
			(cos(points_ratio) + 1)*0.5f,
		};
		vertices[i] = {
			{point_on_circle.x*shape->m_size.x, point_on_circle.y*shape->m_size.y},
			point_on_circle
		};
	}

	shape->loadData(vertices, n);
	delete[] vertices;
}