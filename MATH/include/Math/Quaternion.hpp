#ifndef QUATERNION_CLASS_HEADER
#define QUATERNION_CLASS_HEADER

#include <Math/Vec3.hpp>

namespace mth
{
	class Quaternion
	{
	public:
		float w, x, y, z;

		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(const Quaternion& quat);
		Quaternion(const Vec3& axis, float angle);

		float len() const;
		Quaternion norm(float new_len = 1) const;
		Vec3 rotateVec(const Vec3& vec) const;
	};

	Quaternion operator*(const Quaternion& left, const Quaternion& right);
}

#endif //QUATERNION_CLASS_HEADER