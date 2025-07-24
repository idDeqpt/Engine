#ifndef DRAWABLE_OBJECT_3D_CLASS_HEADER
#define DRAWABLE_OBJECT_3D_CLASS_HEADER

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

	class Object : public Drawable, public mth::Transformable3
	{
	public:
		struct ObjectData
		{
			mth::Vec3* points;
			Color* colors;
			mth::Vec2* tex_coords;
			unsigned int* indexes;
			unsigned int vertices_count;
			unsigned int indexes_count;
		};

		Object();
		~Object();

		bool create();

		void loadData(ObjectData data);
		void setTexture(TextureId texture);

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_VAO;
		unsigned int m_VBO[3];
		unsigned int m_EBO;
		unsigned int m_indexes_count;
		bool m_has_tex_coords;
		TextureId m_texture;
	};
}

#endif //DRAWABLE_OBJECT_3D_CLASS_HEADER