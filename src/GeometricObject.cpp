#include <Graphics/GeometricObject.hpp>

#include <Graphics/Color.hpp>
#include <Math/Vec3.hpp>
#include <cmath>
#include <iostream>


void (*gfx::GeometricObject::generators[])(gfx::GeometricObject*) = {
	parallelepipedGenerator,
	ellipsoidGenerator
};


gfx::GeometricObject::GeometricObject() : m_size(1), m_accuracy(30), Object()
{
	setType(GeometricObject::Type::PARALLELEPIPED);
}

gfx::GeometricObject::GeometricObject(Type type) : m_size(1), m_accuracy(30), Object()
{
	setType(type);
}


void gfx::GeometricObject::setType(const Type new_type)
{
	m_type = new_type;
	m_need_update = true;
	if (new_type == Type::CUSTOM)
		return;
	m_generator = GeometricObject::generators[int(m_type)];
}

void gfx::GeometricObject::setSize(const mth::Vec3& new_size)
{
	m_size = new_size;
	m_need_update = true;
}

void gfx::GeometricObject::setColor(const Color& new_color)
{
	m_color = new_color;
	m_need_update = true;
}

void gfx::GeometricObject::setAccuracy(unsigned int accuracy)
{
	m_accuracy = accuracy;
	m_need_update = true;
}

void gfx::GeometricObject::setGenerator(void (*generator)(GeometricObject*))
{
	m_type = Type::CUSTOM;
	m_generator = generator;
	m_need_update = true;
}


void gfx::GeometricObject::draw(Window* window, RenderStates& states)
{
	update();
	Object::draw(window, states);
}



void gfx::GeometricObject::update()
{
	if (m_need_update)
	{
		m_generator(this);
		m_need_update = false;
	}
}


void gfx::GeometricObject::parallelepipedGenerator(GeometricObject* object)
{
	mth::Vec3 points[8];
	Color colors[8];
	mth::Vec2 tex_coords[8] = {
		{0.25, 0.25},
		{0.75, 0.25},
		{0.25, 0.75},
		{0.75, 0.75},
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1},
	};
	unsigned int indexes[36] = {
		0, 1, 2, 1, 2, 3, //низ
		4, 5, 6, 5, 6, 7, //верх
		0, 1, 5, 0, 5, 4, //перед
		2, 3, 7, 2, 7, 6, //зад
		0, 2, 6, 0, 6, 4, //лево
		1, 3, 7, 1, 7, 5, //право
	};

	for (unsigned int i = 0; i < 8; i++)
	{
		points[i] = mth::Vec3(object->m_size.x*(i % 2), object->m_size.y*((i/2) % 2), object->m_size.z*(((i/2)/2) % 2));
		colors[i] = object->m_color;
	}
	object->loadData({points, colors, tex_coords, indexes, 8, 36});
}

void gfx::GeometricObject::ellipsoidGenerator(GeometricObject* object)
{
	unsigned int n = object->m_accuracy;
	unsigned int points_count = n*n;
	unsigned int indexes_count = (n - 1)*(n - 1)*6;

	mth::Vec3* points = new mth::Vec3[points_count];
	Color* colors = new Color[points_count];
	mth::Vec2* tex_coords = new mth::Vec2[points_count];
	unsigned int* indexes = new unsigned int[indexes_count];

	float pi = 3.14;
	float n_inv = 1.0/(n - 1);
	float pi_div_n = pi*n_inv;

	unsigned int index = 0;
	for (unsigned int i = 0; i < n; i++)
	{
		float theta = i*pi_div_n;
		float sin_th = sin(theta);
		for (unsigned int j = 0; j < n; j++)
		{
			float phi = j*pi_div_n*2;
			mth::Vec3 point(
				object->m_size.x*sin_th*cos(phi),
				object->m_size.y*cos(theta),
				object->m_size.z*sin_th*sin(phi)
			);
			points[index] = point;
			colors[index] = object->m_color;
			tex_coords[index++] = mth::Vec2(float(j) * n_inv, float(i) * n_inv);
		}
	}

	index = 0;
	for (unsigned int i = 0; i < (n - 1); i++)
		for (unsigned int j = 0; j < (n - 1); j++)
		{
			unsigned int first = i*n + j;
			unsigned int second = first + n;

			indexes[index++] = first;
			indexes[index++] = second;
			indexes[index++] = first + 1;

			indexes[index++] = second;
			indexes[index++] = second + 1;
			indexes[index++] = first + 1;
		}
	object->loadData({points, colors, tex_coords, indexes, points_count, indexes_count});

	delete[] points;
	delete[] colors;
	delete[] tex_coords;
	delete[] indexes;
}