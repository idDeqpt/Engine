#ifndef VECTOR_2_HEADER
#define VECTOR_2_HEADER

namespace eng::mth
{
	class Vec2
	{
	public:
		float x;
		float y;

		Vec2();
		Vec2(float v);
		Vec2(float v1, float v2);
		Vec2(const Vec2& vec);

		float len() const;
		float dot(const Vec2& vec) const;
		float cross(const Vec2& vec) const;
		Vec2 norm(float new_len = 1) const;
		Vec2 rotate(float angle) const;

		Vec2& operator=(const Vec2& vec);
		Vec2& operator+=(const Vec2& vec);
		Vec2 operator-() const;
	};

	Vec2 operator+(const Vec2& left, const Vec2& right);
	Vec2 operator-(const Vec2& left, const Vec2& right);
	Vec2 operator*(const Vec2& vec, float coef);
	Vec2 operator*(float coef, const Vec2& vec);
	Vec2 operator/(const Vec2& vec, float coef);
}

#endif //VECTOR_2_HEADER