#include <Engine/Graphics/3D/Mesh.hpp>

#include <Engine/Core/Node3D.hpp>
#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Mat4.hpp>

#include <memory>
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

namespace eng
{

gfx::Mesh::Mesh():
	core::Node3D(),
	m_inited(false),
	m_has_tex_coords(false),
	m_array_buffer(gl::Api::getInstance()->createArrayBuffer())
{}


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

	m_array_buffer->loadData(final_vertices, sizeof(Mesh::Vertex)*last_vertex, gl::ArrayBuffer::Usage::STATIC_DRAW);
	m_array_buffer->configureAttrib(0, 3, false, sizeof(Mesh::Vertex), offsetof(Mesh::Vertex, position));
	m_array_buffer->configureAttrib(1, 2, false, sizeof(Mesh::Vertex), offsetof(Mesh::Vertex, tex_coord));
	m_array_buffer->configureAttrib(2, 3, false, sizeof(Mesh::Vertex), offsetof(Mesh::Vertex, normal));
	m_array_buffer->configureAttrib(3, 3, false, sizeof(Mesh::Vertex), offsetof(Mesh::Vertex, tangent));

	m_array_buffer->configureInstanceAttrib(4, 1, 4, false, sizeof(mth::Mat4), 0);
	m_array_buffer->configureInstanceAttrib(5, 1, 4, false, sizeof(mth::Mat4), sizeof(mth::Mat4)/4);
	m_array_buffer->configureInstanceAttrib(6, 1, 4, false, sizeof(mth::Mat4), sizeof(mth::Mat4)/2);
	m_array_buffer->configureInstanceAttrib(7, 1, 4, false, sizeof(mth::Mat4), (sizeof(mth::Mat4)/4)*3);

	m_array_buffer->loadIndexes(final_indexes, final_vertices_count, gl::ArrayBuffer::Usage::STATIC_DRAW);

	delete[] final_vertices;
	delete[] final_indexes;

	m_has_tex_coords = has_tex_coords;
	return m_inited  = true;
}

void gfx::Mesh::loadInstances(mth::Mat4* transforms, unsigned int transforms_count)
{
	m_array_buffer->loadInstances(transforms, transforms_count);
}


void gfx::Mesh::setMaterial(const Material& new_material)
{
	m_material = new_material;
}


void gfx::Mesh::draw(RenderTarget* target, const RenderStates& states)
{
	if (!m_inited) return;

	Shader* active_shader = gfx::Shader::getActive();

	active_shader->use();
	active_shader->setUniformMatrix4fv("uModel", getGlobalTransform3D().value().getMatrix().getValuesPtr());

	gl::Api::getInstance()->setActiveTexture(0);
		m_material.albedo->bind();
		active_shader->setUniform1i("uMaterial.albedo", 0);

	gl::Api::getInstance()->setActiveTexture(1);
	bool use_normal_map = m_material.normal;
	if (use_normal_map)
	{
		active_shader->setUniform1i("uMaterial.useNormal", use_normal_map);
		m_material.normal->bind();
		active_shader->setUniform1i("uMaterial.normal", 1);
	}

	gl::Api::getInstance()->setActiveTexture(2);
	bool use_metallic_map = m_material.metallic;
	if (use_metallic_map)
	{
		active_shader->setUniform1i("uMaterial.useMetallicic", use_metallic_map);
		m_material.metallic->bind();
		active_shader->setUniform1i("uMaterial.metallic", 2);
	}

	gl::Api::getInstance()->setActiveTexture(3);
	bool use_roughness_map = m_material.roughness;
	if (use_roughness_map)
	{
		active_shader->setUniform1i("uMaterial.useRoughnessic", use_roughness_map);
		m_material.roughness->bind();
		active_shader->setUniform1i("uMaterial.roughness", 3);
	}

	gl::Api::getInstance()->setActiveTexture(4);
	bool use_height_map = m_material.height;
	if (use_height_map)
	{
		active_shader->setUniform1i("uMaterial.useHeight", use_height_map);
		m_material.height->bind();
		active_shader->setUniform1i("uMaterial.height", 4);
	}

	gl::Api::getInstance()->setActiveTexture(5);
	bool use_ao_map = m_material.ao;
	if (use_ao_map)
	{
		active_shader->setUniform1i("uMaterial.useAo", use_ao_map);
		m_material.ao->bind();
		active_shader->setUniform1i("uMaterial.ao", 5);
	}

	gl::Api::getInstance()->setActiveTexture(6);
	bool use_emission_map = m_material.emission;
	if (use_emission_map)
	{
		active_shader->setUniform1i("uMaterial.useEmission", use_emission_map);
		m_material.emission->bind();
		active_shader->setUniform1i("uMaterial.emission", 6);
	}

	m_array_buffer->draw(PrimitiveType::TRIANGLES, 0, 0);
}

} //namespace eng