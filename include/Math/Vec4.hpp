#ifndef VECTOR_4_HEADER
#define VECTOR_4_HEADER

namespace mth
{
	class Vec4
	{
	public:
		float& x = values[0];
		float& y = values[1];
		float& z = values[2];
		float& w = values[3];

		Vec4();
		Vec4(float v);
		Vec4(float v1, float v2, float v3, float v4);
		Vec4(const Vec4& vec);

		float operator[](unsigned int index) const;
		float& operator[](unsigned int index);

	protected:
		float values[4];
	};
}

#endif //VECTOR_4_HEADER