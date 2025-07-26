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
		struct MeshData
		{
			mth::Vec3* points;
			Color* colors;
			mth::Vec2* tex_coords;
			unsigned int* indexes;
			unsigned int vertices_count;
			unsigned int indexes_count;
		};

		Mesh();
		~Mesh();

		bool loadData(MeshData data);
		void setTexture(TextureId texture);

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_VAO;
		unsigned int m_VBO[3];
		unsigned int m_EBO;
		unsigned int m_indexes_count;
		bool m_inited;
		bool m_has_tex_coords;
		TextureId m_texture;
	};
}

#endif //MESH_CLASS_HEADER