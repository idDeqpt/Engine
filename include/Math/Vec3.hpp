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

		Vec4();
		Vec4(float v);
		Vec4(float v1, float v2, float v3);
		Vec4(const Vec3& vec);

		float operator[](unsigned int index) const;
		float& operator[](unsigned int index);

	protected:
		float values[3];
	};
}

#endif //VECTOR_3_HEADER