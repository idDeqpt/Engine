#include <Engine/Graphics/GeometricMesh.hpp>

#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Math/Vec3.hpp>
#include <cmath>
#include <iostream>
#include <string.h>


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
	mth::Vec2 tex_coords[4] = {
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1},
	};
	mth::Vec3 normals[6] = {
		{ 0,  0, -1},
		{ 0,  0,  1},
		{ 0, -1,  0},
		{ 0,  1,  0},
		{-1,  0,  0},
		{ 1,  0,  0},
	};

	unsigned int indexes[36] = {
		3, 2, 1, 2, 0, 1, //зад
		6, 7, 4, 7, 5, 4, //перед
		4, 5, 0, 5, 1, 0, //низ
		2, 6, 3, 6, 7, 3, //верх
		2, 6, 0, 6, 4, 0, //лево
		7, 3, 5, 3, 1, 5, //право
	};
	unsigned int texture_indexes[36];
	unsigned int normal_indexes[36];

	for (unsigned int i = 0; i < 8; i++)
		points[i] = mth::Vec3(mesh->m_size.x*(i % 2), mesh->m_size.y*((i/2) % 2), mesh->m_size.z*(((i/2)/2) % 2));

	for (unsigned int i = 0; i < 6; i++)
	{
		texture_indexes[i*6    ] = 0;
		texture_indexes[i*6 + 1] = 1;
		texture_indexes[i*6 + 2] = 2;
		texture_indexes[i*6 + 3] = 1;
		texture_indexes[i*6 + 4] = 3;
		texture_indexes[i*6 + 5] = 2;
	}
	for (unsigned int i = 0; i < 36; i++)
		normal_indexes[i] = i/6;

	mesh->loadData({points,     8, indexes,
					tex_coords, 4, texture_indexes,
					normals,    6, normal_indexes, 36});
}

void gfx::GeometricMesh::ellipsoidGenerator(GeometricMesh* mesh)
{
	unsigned int n = mesh->m_accuracy;
	unsigned int points_count = n*n;
	unsigned int indexes_count = (n - 1)*(n - 1)*6;

	mth::Vec3* points     = new mth::Vec3[points_count];
	mth::Vec2* tex_coords = new mth::Vec2[points_count];
	mth::Vec3* normals    = new mth::Vec3[points_count];

	unsigned int* indexes       = new unsigned int[indexes_count];

	float pi       = 3.14159265359;
	float n_inv    = 1.0/(n - 1);
	float pi_div_n = pi*n_inv;

	unsigned int index = 0;
	for (unsigned int i = 0; i < n; i++)
	{
		float theta  = i*pi_div_n;
		float sin_th = sin(theta);
		for (unsigned int j = 0; j < n; j++)
		{
			float phi = j*pi_div_n*2;
			mth::Vec3 ell_point = mth::Vec3(sin_th*cos(phi), cos(theta), sin_th*sin(phi));
			mth::Vec3 point(
				mesh->m_size.x*ell_point.x,
				mesh->m_size.y*ell_point.y,
				mesh->m_size.z*ell_point.z
			);
			points[index]     = point;
			tex_coords[index] = mth::Vec2(float(j) * n_inv, float(i) * n_inv);
			normals[index++]  = ell_point;
		}
	}

	index = 0;
	for (unsigned int i = 0; i < (n - 1); i++)
		for (unsigned int j = 0; j < (n - 1); j++)
		{
			unsigned int first  = i*n + j;
			unsigned int second = first + n;

			indexes[index++] = first;
			indexes[index++] = second;
			indexes[index++] = first + 1;

			indexes[index++] = second;
			indexes[index++] = second + 1;
			indexes[index++] = first + 1;
		}

	mesh->loadData({points,     points_count, indexes,
					tex_coords, points_count, indexes,
					normals,    points_count, indexes, indexes_count});

	delete[] points;
	delete[] tex_coords;
	delete[] indexes;
}