#ifndef MESH_CLASS_HEADER
#define MESH_CLASS_HEADER

#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Transformable3.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/Color.hpp>


namespace gfx
{
	class Window;

	class Mesh : public Drawable, public mth::Transformable3
	{
	public:
		struct Vertex
		{
			mth::Vec3 position;
			mth::Vec2 tex_coord;
			mth::Vec3 normal;
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

			unsigned int  vertices_indexes_count;
		};

		Mesh();
		~Mesh();

		bool loadData(MeshData data);
		void setMaterial(const Material& new_material);

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_EBO;
		unsigned int m_indexes_count;
		bool m_inited;
		bool m_has_tex_coords;
		Material m_material;
	};
}

#endif //MESH_CLASS_HEADER