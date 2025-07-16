#ifndef VECTOR_3_HEADER
#define VECTOR_3_HEADER

namespace mth
{
	class Vec3
	{
	public:
		float& x = values[0];
		float& y = values[1];
		float& z = values[2];

		Vec3();
		Vec3(float v);
		Vec3(float v1, float v2, float v3);
		Vec3(const Vec3& vec);

		float len() const;
		Vec3 norm(float new_len = 1) const;

		float operator[](unsigned int index) const;
		float& operator[](unsigned int index);

	protected:
		float values[3];
	};

	Vec3 operator*(const Vec3& vec, float coef);
	Vec3 operator*(float coef, const Vec3& vec);
}

#endif //VECTOR_3_HEADER