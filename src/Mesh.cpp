#include <Graphics/Mesh.hpp>

#include <glad/glad.h>
#include <string>
#include <unordered_map>

#include <Graphics/RenderStates.hpp>
#include <Graphics/TextureManager.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Color.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Transformable3.hpp>


gfx::Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0), m_inited(false), m_has_tex_coords(false), m_texture(0), mth::Transformable3()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

gfx::Mesh::~Mesh()
{
	if (m_VAO)
		glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO)
		glDeleteBuffers(1, &m_VBO);
	if (m_EBO)
		glDeleteBuffers(1, &m_EBO);
}


bool gfx::Mesh::loadData(MeshData data)
{
	if (!(data.unique_posisions && data.posisions_indexes && data.unique_posisions_count && data.vertices_indexes_count)) return m_inited = false;

	unsigned int final_vertices_count = data.vertices_indexes_count;
	unsigned int last_vertex = 0;
	Mesh::Vertex* final_vertices = new Mesh::Vertex[final_vertices_count];
	unsigned int* final_indexes = new unsigned int[final_vertices_count];
	std::unordered_map<std::string, unsigned int> unique_vertices;

	bool has_colors = data.colors_indexes;
	bool has_tex_coords = data.tex_coords_indexes;
	bool has_normals = data.normals_indexes;

	for (unsigned int i = 0; i < final_vertices_count; i++)
	{
		unsigned int pos_id = data.posisions_indexes[i];
		unsigned int col_id = has_colors     ? data.colors_indexes[i]     : 0;
		unsigned int tex_id = has_tex_coords ? data.tex_coords_indexes[i] : 0;
		unsigned int nor_id = has_normals    ? data.normals_indexes[i]    : 0;

		std::string key = std::to_string(pos_id);
		if (has_colors)     key += "_" + col_id;
		if (has_tex_coords) key += "_" + tex_id;
		if (has_normals)    key += "_" + nor_id;

		unsigned int vertex_index;
		auto iter = unique_vertices.find(key);
		if (iter != unique_vertices.end())
			vertex_index = iter->second;
		else
		{
			Mesh::Vertex vertex;
			vertex.position = data.unique_posisions[pos_id];
			vertex.color = has_colors ? data.unique_colors[col_id] : Color(255, 255, 255, 255);
			vertex.tex_coord = has_tex_coords ? data.unique_tex_coords[tex_id] : mth::Vec2(1, 1);
			vertex.normal = has_normals ? data.unique_normals[nor_id] : mth::Vec3(0, 1, 0);

			vertex_index = last_vertex++;
			final_vertices[vertex_index] = vertex;
			unique_vertices[key] = vertex_index;
		}
		final_indexes[i] = vertex_index;
	}

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex)*last_vertex, final_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, tex_coord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*final_vertices_count, final_indexes, GL_STATIC_DRAW);
	m_indexes_count = final_vertices_count;
	
	glBindVertexArray(0);

	delete[] final_vertices;
	delete[] final_indexes;
	m_has_tex_coords = has_tex_coords;
	return m_inited = true;
}

void gfx::Mesh::setTexture(TextureId texture)
{
	m_texture = texture;
}


void gfx::Mesh::draw(Window* window, RenderStates& states)
{
	if (!m_inited) return;

	states.m_transform = getGlobalTransform();
	states.m_texture = m_texture;

	bool use_texture = states.m_texture && m_has_tex_coords;

	states.m_shader->use();
	states.m_shader->setUniform1i("use_texture", use_texture);
	if (use_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		TextureManager::bind(states.m_texture);
		states.m_shader->setUniformMatrix4fv("u_texture", 0);
	}
	states.m_shader->setUniformMatrix4fv("projection", states.m_view.getProjectionMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("view", states.m_view.getViewMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("model", states.m_transform.getMatrix().getValuesPtr());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexes_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}