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


struct VertexKey {
	unsigned int positionIndex;
	unsigned int textureCoordIndex;
	unsigned int normalIndex;

	friend bool operator==(const VertexKey& lhs, const VertexKey& rhs) noexcept {
		return (lhs.positionIndex     == rhs.positionIndex     &&
				lhs.textureCoordIndex == rhs.textureCoordIndex &&
				lhs.normalIndex       == rhs.normalIndex);
	}
};

template <>
struct std::hash<VertexKey> {
	size_t operator()(const VertexKey &key) const noexcept {
		return std::hash<unsigned int>{}(key.positionIndex)           ^
			  (std::hash<unsigned int>{}(key.textureCoordIndex) << 1) ^
			  (std::hash<unsigned int>{}(key.normalIndex)       >> 1);
	}
};


gfx::Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0), m_instance_VBO(0), m_inited(false), m_has_tex_coords(false), mth::Transformable3()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
	glGenBuffers(1, &m_instance_VBO);

	loadInstances(nullptr, 0);
}

gfx::Mesh::~Mesh()
{
	if (m_VAO)
		glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO)
		glDeleteBuffers(1, &m_VBO);
	if (m_EBO)
		glDeleteBuffers(1, &m_EBO);
	if (m_instance_VBO)
		glDeleteBuffers(1, &m_instance_VBO);
}


bool gfx::Mesh::loadData(MeshData data)
{
	if (!(data.unique_posisions && data.posisions_indexes && data.unique_posisions_count && data.vertices_indexes_count)) return m_inited = false;

	unsigned int  final_vertices_count = data.vertices_indexes_count;
	unsigned int  last_vertex          = 0;
	Mesh::Vertex* final_vertices       = new Mesh::Vertex[final_vertices_count];
	unsigned int* final_indexes        = new unsigned int[final_vertices_count];
	std::unordered_map<VertexKey, unsigned int> unique_vertices;

	bool has_tex_coords = data.unique_tex_coords;
	bool has_normals    = data.unique_normals;
	bool has_tangents   = data.unique_tangents;

	for (unsigned int i = 0; i < final_vertices_count; i++)
	{
		unsigned int pos_id = data.posisions_indexes[i];
		unsigned int tex_id = has_tex_coords ? data.tex_coords_indexes[i] : 0;
		unsigned int nor_id = has_normals    ? data.normals_indexes[i]    : 0;
		unsigned int tan_id = has_tangents   ? data.tangents_indexes[i]   : 0;

		VertexKey key {pos_id, tex_id, nor_id};

		unsigned int vertex_index;
		auto iter = unique_vertices.find(key);
		if (iter != unique_vertices.end())
			vertex_index = iter->second;
		else
		{
			Mesh::Vertex vertex;
			vertex.position  = data.unique_posisions[pos_id];
			vertex.tex_coord = has_tex_coords ? data.unique_tex_coords[tex_id] : mth::Vec2(1, 1);
			vertex.normal    = has_normals    ? data.unique_normals[nor_id]    : mth::Vec3(0, 1, 0);
			vertex.tangent   = has_tangents   ? data.unique_tangents[nor_id]   : mth::Vec3(1, 0, 0);

			vertex_index                 = last_vertex++;
			final_vertices[vertex_index] = vertex;
			unique_vertices[key]         = vertex_index;
		}
		final_indexes[i] = vertex_index;
	}

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex)*last_vertex, final_vertices, GL_STATIC_DRAW);
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, tex_coord));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, normal));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, tangent));
		glEnableVertexAttribArray(3);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_instance_VBO);
	{
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mth::Mat4), (GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);

		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mth::Mat4), (GLvoid*)(sizeof(mth::Mat4)/4));
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);

		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(mth::Mat4), (GLvoid*)(sizeof(mth::Mat4)/2));
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(6, 1);

		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(mth::Mat4), (GLvoid*)((sizeof(mth::Mat4)/4)*3));
		glEnableVertexAttribArray(7);
		glVertexAttribDivisor(7, 1);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*final_vertices_count, final_indexes, GL_STATIC_DRAW);
	m_indexes_count = final_vertices_count;
	
	glBindVertexArray(0);

	delete[] final_vertices;
	delete[] final_indexes;

	m_has_tex_coords = has_tex_coords;
	return m_inited  = true;
}

void gfx::Mesh::loadInstances(mth::Mat4* transforms, unsigned int transforms_count)
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_instance_VBO);

	if (!transforms || !transforms_count)
	{
		mth::Mat4 identity = mth::Mat4::getIdentity();
		glBufferData(GL_ARRAY_BUFFER, sizeof(mth::Mat4), &identity, GL_STATIC_DRAW);
		m_instances_count = 1;
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(mth::Mat4)*transforms_count, transforms, GL_STATIC_DRAW);
		m_instances_count = transforms_count;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	bool use_normal_map = m_material.normal;
		active_shader->setUniform1i("uMaterial.useNormal", use_normal_map);
		glActiveTexture(GL_TEXTURE2);
		Texture::bind(m_material.normal);
		active_shader->setUniform1i("uMaterial.normal", 2);
	bool use_parallax_map = m_material.parallax;
		active_shader->setUniform1i("uMaterial.useParallax", use_parallax_map);
		glActiveTexture(GL_TEXTURE3);
		Texture::bind(m_material.parallax);
		active_shader->setUniform1i("uMaterial.parallax", 3);
	active_shader->setUniform1f("uMaterial.shininess", m_material.shininess);

	active_shader->setUniform3fv("uViewPos", 1, &view_glob_pos.x);

	LightManager::DirectionalLight light = LightManager::getDirectionalLight();
	bool use_directional_light = light.direction.x || light.direction.y || light.direction.z;
	active_shader->setUniform1i("uUseDirectionalLight", use_directional_light);
	if (use_directional_light)
	{
		active_shader->setUniform3fv("uDirectionalLight.direction", 1, &light.direction.x);
		active_shader->setUniform3fv("uDirectionalLight.color", 1, &light.color.x);
	}

	glBindVertexArray(m_VAO);
	glDrawElementsInstanced(GL_TRIANGLES, m_indexes_count, GL_UNSIGNED_INT, 0, m_instances_count);
	glBindVertexArray(0);
}