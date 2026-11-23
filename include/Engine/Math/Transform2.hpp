#ifndef TRANSFORM_2_CLASS_HEADER
#define TRANSFORM_2_CLASS_HEADER

#include <Engine/Math/Mat.hpp>
#include <Engine/Math/Vec.hpp>

namespace eng::mth
{
	class Transform2
	{
	public:
		Transform2() = default;
		Transform2(const Mat3& mat);

		void translate(Vec2 vec);
		void scale(Vec2 vec);
		void rotate(float angle); //radians

		const Mat3& getMatrix() const;

	protected:
		Mat3 m_matrix = Mat3::getIdentity();
	};
}

#endif //TRANSFORM_2_CLASS_HEADER