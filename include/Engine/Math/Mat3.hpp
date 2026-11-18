#ifndef MATRIX_3X3_CLASS_HEADER
#define MATRIX_3X3_CLASS_HEADER

#include <Engine/Math/Vec.hpp>

namespace eng::mth
{
	class Mat3
	{
	public:
		class Row
		{
		public:
			Row(float* values);

			float operator[](unsigned int index) const;
			float& operator[](unsigned int index);

		protected:
			float* values;
		};

		float values[9];

		Mat3();
		Mat3(float v);
		Mat3(const Mat3& mat);
		Mat3(float v00, float v01, float v02,
			 float v10, float v11, float v12,
			 float v20, float v21, float v22);

		float det() const;
		bool invert(Mat3& out) const;

		const Row operator[](unsigned int index) const;
		Row operator[](unsigned int index);

		static Mat3 getIdentity();
	};

	inline Mat3 operator*(const Mat3& a, const Mat3& b)
	{
		Mat3 r;
		for (int i = 0; i < 3; i++)
		{
			const float* ar = a.values + i*3;
			float*       rr = r.values + i*3;

			const float a0 = ar[0];
			const float a1 = ar[1];
			const float a2 = ar[2];

			rr[0] = a0*b.values[0] + a1*b.values[3] + a2*b.values[6];
			rr[1] = a0*b.values[1] + a1*b.values[4] + a2*b.values[7];
			rr[2] = a0*b.values[2] + a1*b.values[5] + a2*b.values[8];
		}
		return r;
	}

	inline Vec3 operator*(const Mat3& left, const Vec3& right)
	{
		return {
			left.values[0*3 + 0]*right.x + left.values[0*3 + 1]*right.y + left.values[0*3 + 2]*right.z,
			left.values[1*3 + 0]*right.x + left.values[1*3 + 1]*right.y + left.values[1*3 + 2]*right.z,
			left.values[2*3 + 0]*right.x + left.values[2*3 + 1]*right.y + left.values[2*3 + 2]*right.z
		};
	}
}

#endif //MATRIX_3X3_CLASS_HEADER