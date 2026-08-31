#ifndef MESH_CLASS_HEADER
#define MESH_CLASS_HEADER

#include <Engine/Core/Node3D.hpp>
#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Mat4.hpp>

#include <memory>


namespace eng::gfx
{
	class RenderTarget;

	class Mesh : public Drawable, public core::Node3D
	{
	public:
		struct Vertex
		{
			mth::Vec3 position;
			mth::Vec2 tex_coord;
			mth::Vec3 normal;
			mth::Vec3 tangent;
		};

		struct MeshData
		{
			mth::Vec3*    unique_posisions;
			unsigned int  unique_posisions_count;
			unsigned int* posisions_indexes;

			mth::Vec2*    unique_tex_coords;
			unsigned int  unique_tex_coords_count;
			unsigned int* tex_coords_indexes;

			mth::Vec3*    unique_normals;
			unsigned int  unique_normals_count;
			unsigned int* normals_indexes;

			mth::Vec3*    unique_tangents;
			unsigned int  unique_tangents_count;
			unsigned int* tangents_indexes;

			unsigned int  vertices_indexes_count;
		};

		Mesh();
		~Mesh() = default;

		bool loadData(MeshData data);
		void loadInstances(mth::Mat4* transforms, unsigned int transforms_count);
		void setMaterial(const Material& new_material);

		void draw(RenderTarget* target, const RenderStates& states);

	protected:
		std::unique_ptr<gl::ArrayBuffer> m_array_buffer;
		bool m_inited;
		bool m_has_tex_coords;
		Material m_material;
	};
}

#endif //MESH_CLASS_HEADER