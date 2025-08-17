#ifndef VECTOR_3_HEADER
#define VECTOR_3_HEADER

namespace mth
{
	class Vec3
	{
	public:
		float x;
		float y;
		float z;

		Vec3();
		Vec3(float v);
		Vec3(float v1, float v2, float v3);
		Vec3(const Vec3& vec);

		float len() const;
		Vec3 norm(float new_len = 1) const;
		Vec3 cross(const Vec3& vec);

		float operator[](unsigned int index) const;
		float& operator[](unsigned int index);
		Vec3& operator=(const Vec3& vec);
		Vec3& operator+=(const Vec3& vec);
		Vec3 operator-() const;
	};

	Vec3 operator-(const Vec3& left, const Vec3& rigth);
	Vec3 operator*(const Vec3& vec, float coef);
	Vec3 operator*(float coef, const Vec3& vec);
	Vec3 operator/(const Vec3& vec, float coef);
}

#endif //VECTOR_3_HEADER