#ifndef TRANSFORM_2_CLASS_HEADER
#define TRANSFORM_2_CLASS_HEADER

#include <Math/Mat3.hpp>
#include <Math/Vec2.hpp>

namespace mth
{
	class Transform2
	{
	public:
		Transform2();
		Transform2(const Mat3& mat);
		Transform2(Transform2& transform);

		void translate(const Vec2& vec);
		void scale(const Vec2& vec);
		void rotate(float angle); //radians

		Mat3 getMatrix();

	protected:
		Mat3 m_matrix;
	};
}

#endif //TRANSFORM_2_CLASS_HEADER