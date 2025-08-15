#ifndef VECTOR_2_HEADER
#define VECTOR_2_HEADER

namespace mth
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

		Vec2& operator=(const Vec2& vec);
		Vec2& operator+=(const Vec2& vec);
		Vec2 operator-() const;
	};

	Vec2 operator+(const Vec2& left, const Vec2& right);
	Vec2 operator-(const Vec2& left, const Vec2& right);
}

#endif //VECTOR_2_HEADER