#include <Engine/Graphics/Mesh.hpp>

#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/Graphics/Window.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Vec4.hpp>
#include <Engine/Math/Mat4.hpp>
#include <Engine/Math/Transformable3.hpp>

#include <glad/glad.h>
#include <string>
#include <unordered_map>


gfx::Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0), m_inited(false), m_has_tex_coords(false), mth::Transformable3()
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

	bool has_tex_coords = data.unique_tex_coords;
	bool has_normals = data.unique_normals;

	for (unsigned int i = 0; i < final_vertices_count; i++)
	{
		unsigned int pos_id = data.posisions_indexes[i];
		unsigned int tex_id = has_tex_coords ? data.tex_coords_indexes[i] : 0;
		unsigned int nor_id = has_normals    ? data.normals_indexes[i]    : 0;

		std::string key = std::to_string(pos_id);
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

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, tex_coord));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, normal));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*final_vertices_count, final_indexes, GL_STATIC_DRAW);
	m_indexes_count = final_vertices_count;
	
	glBindVertexArray(0);

	delete[] final_vertices;
	delete[] final_indexes;

	m_has_tex_coords = has_tex_coords;
	return m_inited = true;
}

void gfx::Mesh::setMaterial(const Material& new_material)
{
	m_material = new_material;
}


void gfx::Mesh::draw(Window* window, RenderStates& states)
{
	if (!m_inited) return;

	View* active_view = gfx::View::getActive();
	Shader* active_shader = gfx::Shader::getActive();
	mth::Vec3 view_loc_pos = active_view->getPosition();
	mth::Mat4 view_transform_mat = active_view->getGlobalTransform().getMatrix();
	mth::Vec4 view_glob_pos = view_transform_mat*mth::Vec4(view_loc_pos.x, view_loc_pos.y, view_loc_pos.z, 1);

	active_shader->use();
	active_shader->setUniformMatrix4fv("uProjection", active_view->getProjectionMatrix().getValuesPtr());
	active_shader->setUniformMatrix4fv("uView", active_view->getViewMatrix().getValuesPtr());
	active_shader->setUniformMatrix4fv("uModel", getGlobalTransform().getMatrix().getValuesPtr());

	glActiveTexture(GL_TEXTURE0);
	Texture::bind(m_material.diffuse);
	active_shader->setUniform1i("uMaterial.diffuse", 0);
	glActiveTexture(GL_TEXTURE1);
	Texture::bind(m_material.specular);
	active_shader->setUniform1i("uMaterial.specular", 1);
	active_shader->setUniform1f("uMaterial.shininess", m_material.shininess);

	active_shader->setUniform3fv("uViewPos", 1, &view_glob_pos.x);

	active_shader->setUniform1i("uLightsCount", LightManager::getLightsCount());
	if (LightManager::getLightsCount())
	{
		glActiveTexture(GL_TEXTURE2);
		Texture::bind(LightManager::getLightsTexture());
		active_shader->setUniform1i("uLightsTexture", 2);
	}

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexes_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}