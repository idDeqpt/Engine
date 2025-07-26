#ifndef GEAOMETRIC_MESH_CLASS_HEADER
#define GEAOMETRIC_MESH_CLASS_HEADER

#include <Graphics/GeometricMesh.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Color.hpp>
#include <Math/Vec3.hpp>

namespace gfx
{
	class GeometricMesh : public Mesh
	{
	public:
		enum class Type
		{
			PARALLELEPIPED,
			ELLIPSOID,
			CUSTOM,
		};

		GeometricMesh();
		GeometricMesh(Type type);

		void setType(const Type new_type);
		void setSize(const mth::Vec3& new_size);
		void setColor(const Color& new_color);
		void setAccuracy(unsigned int accuracy);
		void setGenerator(void (*generator)(GeometricMesh*));

		void draw(Window* window, RenderStates& states);

	protected:
		Type m_type;
		mth::Vec3 m_size;
		Color m_color;
		unsigned int m_accuracy;
		bool m_need_update;
		void (*m_generator)(GeometricMesh*) = parallelepipedGenerator;

		void update();

		static void (*generators[])(GeometricMesh*);
		static void parallelepipedGenerator(GeometricMesh* mesh);
		static void ellipsoidGenerator(GeometricMesh* mesh);
	};
}

#endif //GEAOMETRIC_MESH_CLASS_HEADER