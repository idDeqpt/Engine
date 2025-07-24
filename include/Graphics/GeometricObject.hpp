#ifndef GEAOMETRIC_DRAWABLE_OBJECT_CLASS_HEADER
#define GEAOMETRIC_DRAWABLE_OBJECT_CLASS_HEADER

#include <Graphics/GeometricObject.hpp>
#include <Graphics/Object.hpp>
#include <Graphics/Color.hpp>
#include <Math/Vec3.hpp>

namespace gfx
{
	class GeometricObject : public Object
	{
	public:
		enum class Type
		{
			PARALLELEPIPED,
			ELLIPSOID,
			CUSTOM,
		};

		GeometricObject();
		GeometricObject(Type type);

		void setType(const Type new_type);
		void setSize(const mth::Vec3& new_size);
		void setColor(const Color& new_color);
		void setAccuracy(unsigned int accuracy);
		void setGenerator(void (*generator)(GeometricObject*));

		void draw(Window* window, RenderStates& states);

	protected:
		Type m_type;
		mth::Vec3 m_size;
		Color m_color;
		unsigned int m_accuracy;
		bool m_need_update;
		void (*m_generator)(GeometricObject*) = parallelepipedGenerator;

		void update();

		static void (*generators[])(GeometricObject*);
		static void parallelepipedGenerator(GeometricObject* object);
		static void ellipsoidGenerator(GeometricObject* object);
	};
}

#endif //GEAOMETRIC_DRAWABLE_OBJECT_CLASS_HEADER