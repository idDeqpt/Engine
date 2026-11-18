#ifndef VECTOR_TEMPLATE_CLASS_HEADER
#define VECTOR_TEMPLATE_CLASS_HEADER

namespace eng::mth
{
	template<unsigned int N>
	struct VecStorage
	{
		float values[N] = {};

		float& operator[](unsigned int i);
		const float& operator[](unsigned int i) const;
	};

	template<>
	struct VecStorage<2>
	{
		float x = 0;
		float y = 0;

		inline float& operator[](unsigned int i);
		inline const float& operator[](unsigned int i) const;
	};

	template<>
	struct VecStorage<3>
	{
		float x = 0;
		float y = 0;
		float z = 0;
		
		inline float& operator[](unsigned int i);
		inline const float& operator[](unsigned int i) const;
	};

	template<>
	struct VecStorage<4>
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
		
		inline float& operator[](unsigned int i);
		inline const float& operator[](unsigned int i) const;
	};

	template<unsigned int N>
	struct Vec : public VecStorage<N>
	{
		Vec() = default;
		Vec(float v);
		Vec(float v1, float v2);
		Vec(float v1, float v2, float v3);
		Vec(float v1, float v2, float v3, float v4);
		Vec(const Vec& vec) = default;

		Vec& operator=(const Vec& vec) = default;
		Vec& operator+=(const Vec& vec);
		Vec  operator-() const;
	};

	using Vec2 = Vec<2>;
	using Vec3 = Vec<3>;
	using Vec4 = Vec<4>;

	template<unsigned int N>  float  len(const Vec<N>& vec);
	template<unsigned int N>  float  dot(const Vec<N>& left, const Vec<N>& right);
	template<unsigned int N>  Vec<N> norm(const Vec<N>& vec, float new_len = 1);

	inline float cross(const Vec2& left, const Vec2& right);
	inline Vec2 rotate(const Vec2& vec, float angle);
	inline Vec3 cross(const Vec3& left, const Vec3& right);

	template<unsigned int N>  Vec<N> operator+(const Vec<N>& left, const Vec<N>& right);
	template<unsigned int N>  Vec<N> operator-(const Vec<N>& left, const Vec<N>& right);
	template<unsigned int N>  Vec<N> operator+(const Vec<N>& vec, float v);
	template<unsigned int N>  Vec<N> operator-(const Vec<N>& vec, float v);
	template<unsigned int N>  Vec<N> operator*(const Vec<N>& vec, float coef);
	template<unsigned int N>  Vec<N> operator*(float coef, const Vec<N>& vec);
	template<unsigned int N>  Vec<N> operator/(const Vec<N>& vec, float coef);
}

#include <Engine/Math/Vec.inl>

#endif //VECTOR_TEMPLATE_CLASS_HEADER