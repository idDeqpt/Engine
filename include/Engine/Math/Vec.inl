#include <cassert>
#include <cmath>


namespace eng::mth
{

template<unsigned int N>
float& VecStorage<N>::operator[](unsigned int i)
{
	assert(i < N);
	return values[i];
}

template<unsigned int N>
const float& VecStorage<N>::operator[](unsigned int i) const
{
	assert(i < N);
	return values[i];
}


inline float& VecStorage<2>::operator[](unsigned int i)
{
	assert(i < 2);
	switch (i)
	{
		case 0:  return x;
		default: return y;
	}
}

inline const float& VecStorage<2>::operator[](unsigned int i) const
{
	assert(i < 2);
	switch (i)
	{
		case 0:  return x;
		default: return y;
	}
}


inline float& VecStorage<3>::operator[](unsigned int i)
{
	assert(i < 3);
	switch (i)
	{
		case 0:  return x;
		case 1:  return y;
		default: return z;
	}
}

inline const float& VecStorage<3>::operator[](unsigned int i) const
{
	assert(i < 3);
	switch (i)
	{
		case 0:  return x;
		case 1:  return y;
		default: return z;
	}
}


inline float& VecStorage<4>::operator[](unsigned int i)
{
	assert(i < 4);
	switch (i)
	{
		case 0:  return x;
		case 1:  return y;
		case 2:  return z;
		default: return w;
	}
}

inline const float& VecStorage<4>::operator[](unsigned int i) const
{
	assert(i < 4);
	switch (i)
	{
		case 0:  return x;
		case 1:  return y;
		case 2:  return z;
		default: return w;
	}
}



template<unsigned int N>
Vec<N>::Vec(float v)
{
	for (unsigned int i = 0; i < N; i++)
		(*this)[i] = v;
}

template<unsigned int N>
Vec<N>::Vec(float v1, float v2)
{
	static_assert(N == 2);

	(*this)[0] = v1;
	(*this)[1] = v2;
}

template<unsigned int N>
Vec<N>::Vec(float v1, float v2, float v3)
{
	static_assert(N == 3);

	(*this)[0] = v1;
	(*this)[1] = v2;
	(*this)[2] = v3;
}

template<unsigned int N>
Vec<N>::Vec(float v1, float v2, float v3, float v4)
{
	static_assert(N == 4);

	(*this)[0] = v1;
	(*this)[1] = v2;
	(*this)[2] = v3;
	(*this)[3] = v4;
}


template<unsigned int N>
Vec<N>& Vec<N>::operator+=(const Vec<N>& vec)
{
	for (unsigned int i = 0; i < N; i++)
		(*this)[i] += vec[i];
	return *this;
}

template<unsigned int N>
Vec<N> Vec<N>::operator-() const
{
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = -(*this)[i];
	return res;
}



template<unsigned int N>
float len(const Vec<N>& vec)
{
	return std::sqrt(dot(vec, vec));
}

template<unsigned int N>
float dot(const Vec<N>& left, const Vec<N>& right)
{
	float sum = 0;
	for (unsigned int i = 0; i < N; i++)
		sum += left[i]*right[i];
	return sum;
}

template<unsigned int N>
Vec<N> norm(const Vec<N>& vec, float new_len)
{
	assert(len(vec) != 0);
	float coef = new_len/len(vec);
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = vec[i]*coef;
	return res;
}


inline float cross(Vec2 left, Vec2 right)
{
	return left.x*right.y - left.y*right.x;
}

inline Vec2 rotate(Vec2 vec, float angle)
{
	float cos = std::cos(angle);
	float sin = std::sin(angle);

	return {
		vec.x*cos - vec.y*sin,
		vec.x*sin + vec.y*cos
	};
}

inline Vec3 cross(const Vec3& left, const Vec3& right)
{
	return {
		left.y*right.z - left.z*right.y,
        left.z*right.x - left.x*right.z,
        left.x*right.y - left.y*right.x
	};
}



template<unsigned int N>
Vec<N> operator+(const Vec<N>& left, const Vec<N>& right)
{
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = left[i] + right[i];
	return res;
}

template<unsigned int N>
Vec<N> operator-(const Vec<N>& left, const Vec<N>& right)
{
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = left[i] - right[i];
	return res;
}

template<unsigned int N>
Vec<N> operator+(const Vec<N>& vec, float v)
{
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = vec[i] + v;
	return res;
}

template<unsigned int N>
Vec<N> operator-(const Vec<N>& vec, float v)
{
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = vec[i] - v;
	return res;
}

template<unsigned int N>
Vec<N> operator*(const Vec<N>& vec, float coef)
{
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = vec[i] * coef;
	return res;
}

template<unsigned int N>
Vec<N> operator*(float coef, const Vec<N>& vec)
{
	return vec*coef;
}

template<unsigned int N>
Vec<N> operator/(const Vec<N>& vec, float coef)
{
	assert(coef != 0);
	float v = 1/coef;
	Vec<N> res;
	for (unsigned int i = 0; i < N; i++)
		res[i] = vec[i] * v;
	return res;

}

} //namespace eng::mth