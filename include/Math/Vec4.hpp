#ifndef VECTOR_4_HEADER
#define VECTOR_4_HEADER

namespace mth
{
	class Vec4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vec4();
		Vec4(float v);
		Vec4(float v1, float v2, float v3, float v4);
		Vec4(const Vec4& vec);

		float operator[](unsigned int index) const;
		float& operator[](unsigned int index);
	};
}

#endif //VECTOR_4_HEADER