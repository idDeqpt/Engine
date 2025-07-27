#include <Graphics/GeometricMesh.hpp>

#include <Graphics/Color.hpp>
#include <Math/Vec3.hpp>
#include <cmath>
#include <iostream>


void (*gfx::GeometricMesh::generators[])(gfx::GeometricMesh*) = {
	parallelepipedGenerator,
	ellipsoidGenerator
};


gfx::GeometricMesh::GeometricMesh() : m_size(1), m_accuracy(30), Mesh()
{
	setType(GeometricMesh::Type::PARALLELEPIPED);
}

gfx::GeometricMesh::GeometricMesh(Type type) : m_size(1), m_accuracy(30), Mesh()
{
	setType(type);
}


void gfx::GeometricMesh::setType(const Type new_type)
{
	m_type = new_type;
	m_need_update = true;
	if (new_type == Type::CUSTOM)
		return;
	m_generator = GeometricMesh::generators[int(m_type)];
}

void gfx::GeometricMesh::setSize(const mth::Vec3& new_size)
{
	m_size = new_size;
	m_need_update = true;
}

void gfx::GeometricMesh::setColor(const Color& new_color)
{
	m_color = new_color;
	m_need_update = true;
}

void gfx::GeometricMesh::setAccuracy(unsigned int accuracy)
{
	m_accuracy = accuracy;
	m_need_update = true;
}

void gfx::GeometricMesh::setGenerator(void (*generator)(GeometricMesh*))
{
	m_type = Type::CUSTOM;
	m_generator = generator;
	m_need_update = true;
}


void gfx::GeometricMesh::draw(Window* window, RenderStates& states)
{
	update();
	Mesh::draw(window, states);
}



void gfx::GeometricMesh::update()
{
	if (m_need_update)
	{
		m_generator(this);
		m_need_update = false;
	}
}


void gfx::GeometricMesh::parallelepipedGenerator(GeometricMesh* mesh)
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
		points[i] = mth::Vec3(mesh->m_size.x*(i % 2), mesh->m_size.y*((i/2) % 2), mesh->m_size.z*(((i/2)/2) % 2));
		colors[i] = mesh->m_color;
	}
	mesh->loadData({points, 8, indexes, colors, 8, indexes, tex_coords, 8, indexes, nullptr, 0, nullptr, 36});
}

void gfx::GeometricMesh::ellipsoidGenerator(GeometricMesh* mesh)
{
	unsigned int n = mesh->m_accuracy;
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
				mesh->m_size.x*sin_th*cos(phi),
				mesh->m_size.y*cos(theta),
				mesh->m_size.z*sin_th*sin(phi)
			);
			points[index] = point;
			colors[index] = mesh->m_color;
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
	mesh->loadData({points, points_count, indexes,
					colors, points_count, indexes,
					tex_coords, points_count, indexes,
					nullptr, 0, nullptr, indexes_count});

	delete[] points;
	delete[] colors;
	delete[] tex_coords;
	delete[] indexes;
}