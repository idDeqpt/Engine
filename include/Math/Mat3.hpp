#ifndef MATRIX_3X3_CLASS_HEADER
#define MATRIX_3X3_CLASS_HEADER

namespace mth
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

		Mat3();
		Mat3(float v);
		Mat3(const Mat3& mat);
		Mat3(float v00, float v01, float v02,
			 float v10, float v11, float v12,
			 float v20, float v21, float v22);

		float det() const;

		float* getValuesPtr();

		const Row operator[](unsigned int index) const;
		Row operator[](unsigned int index);

		static Mat3 getIdentity();

	protected:
		float values[9];
	};

	Mat3 operator*(const Mat3& left, const Mat3& right);
}

#endif //MATRIX_3X3_CLASS_HEADER