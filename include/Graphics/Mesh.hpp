#ifndef MESH_CLASS_HEADER
#define MESH_CLASS_HEADER

#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Transformable3.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/Drawable.hpp>
#include <Graphics/Color.hpp>


namespace gfx
{
	class Vertex;
	class Window;
	class Texture;

	class Mesh : public Drawable, public mth::Transformable3
	{
	public:
		struct Vertex
		{
			mth::Vec3 position;
			Color     color;
			mth::Vec2 tex_coord;
			mth::Vec3 normal;
		};

		struct MeshData
		{
			mth::Vec3*    unique_posisions;
			unsigned int  unique_posisions_count;
			unsigned int* posisions_indexes;

			Color*        unique_colors;
			unsigned int  unique_colors_count;
			unsigned int* colors_indexes;

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
		void setTexture(TextureId texture);

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_EBO;
		unsigned int m_indexes_count;
		bool m_inited;
		bool m_has_tex_coords;
		TextureId m_texture;
	};
}

#endif //MESH_CLASS_HEADER