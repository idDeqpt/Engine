#ifndef DRAWABLE_OBJECT_3D_CLASS_HEADER
#define DRAWABLE_OBJECT_3D_CLASS_HEADER

#include <Math/Transformable3.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/Drawable.hpp>


namespace gfx
{
	class Vertex;
	class Window;
	class Texture;

	class Object : public Drawable, public mth::Transformable3
	{
	public:
		Object();
		~Object();

		bool create();

		bool updateData(Vertex* vertices, unsigned int vertices_count, unsigned int* indexes, unsigned int indexies_count);

		void setTexture(TextureId texture);

		void draw(Window* window, RenderStates& states);

	protected:
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_EBO;
		unsigned int m_indexes_count;
		TextureId m_texture;
	};
}

#endif //DRAWABLE_OBJECT_3D_CLASS_HEADER