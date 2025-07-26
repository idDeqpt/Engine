#ifndef TRANSFORM_3_CLASS_HEADER
#define TRANSFORM_3_CLASS_HEADER

#include <Math/Mat4.hpp>
#include <Math/Vec3.hpp>
#include <Math/Quaternion.hpp>

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
		void rotate(const Quaternion& quat);

		Mat4 getMatrix();

		static Mat4 getIdentity();

	protected:
		Mat4 m_matrix;
	};
}

#endif //TRANSFORM_3_CLASS_HEADER