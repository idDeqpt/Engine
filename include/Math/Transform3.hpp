#ifndef TRANSFORM_3_CLASS_HEADER
#define TRANSFORM_3_CLASS_HEADER

#include "Mat4.hpp"
#include "Vec3.hpp"

namespace mth
{
	class Transform3
	{
	public:
		Transform3();
		Transform3(const Mat4& mat);
		Transform3(Transform3& transform);

		void translate(const Vec3& vec);
		void scale(const Vec3& vec);
		void rotate(const Vec3& vec, float angle); //radians

		Mat4 getMatrix();

		static Mat4 getIdentity();

	protected:
		Mat4 m_matrix;
	};
}

#endif //TRANSFORM_3_CLASS_HEADER