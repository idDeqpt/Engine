#include <Math/Quaternion.hpp>

#include <Math/Vec3.hpp>
#include <cmath>


mth::Quaternion::Quaternion():
	w(1), x(0), y(0), z(0) {}

mth::Quaternion::Quaternion(float w, float x, float y, float z):
	w(w), x(x), y(y), z(z) {}

mth::Quaternion::Quaternion(const Quaternion& quat):
	w(quat.w), x(quat.x), y(quat.y), z(quat.z) {}

mth::Quaternion::Quaternion(const Vec3& axis, float angle)
{
	float halfAngle = angle/2;
	float sinh = sin(halfAngle);
	Vec3 normAxis = axis.norm();

	x = normAxis.x * sinh;
	y = normAxis.y * sinh;
	z = normAxis.z * sinh;
	w = cos(halfAngle);
	*this = norm();
}


float mth::Quaternion::len() const
{
	return sqrt(x*x + y*y + z*z + w*w);
}

mth::Quaternion mth::Quaternion::norm(float new_len) const
{
	float coef = 1.0/len();
	return Quaternion(w*coef, x*coef, y*coef, z*coef);
}

mth::Vec3 mth::Quaternion::rotateVec(const Vec3& vec) const
{
	Quaternion normed = this->norm();
	Quaternion p(0, vec.x, vec.y, vec.z);
	Quaternion conjugate(normed.w, -normed.x, -normed.y, -normed.z);
	Quaternion result = (normed * p)*conjugate;
	return Vec3(result.x, result.y, result.z);
}


mth::Quaternion mth::operator*(const Quaternion& left, const Quaternion& right)
{
	return Quaternion(
		left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z,
		left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y,
		left.w * right.y - left.x * right.z + left.y * right.w + left.z * right.x,
		left.w * right.z + left.x * right.y - left.y * right.x + left.z * right.w
	);
}