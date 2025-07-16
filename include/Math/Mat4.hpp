#ifndef MATRIX_4X4_CLASS_HEADER
#define MATRIX_4X4_CLASS_HEADER

namespace mth
{
	class Mat4
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

		Mat4();
		Mat4(float v);
		Mat4(const Mat4& mat);
		Mat4(float v00, float v01, float v02, float v03,
			 float v10, float v11, float v12, float v13,
			 float v20, float v21, float v22, float v23,
			 float v30, float v31, float v32, float v33);

		float* getValuesPtr();

		const Row operator[](unsigned int index) const;
		Row operator[](unsigned int index);
		Mat4& operator=(const Mat4& mat);

	protected:
		float values[16];
	};

	Mat4 operator*(const Mat4& left, const Mat4& right);
}

#endif //MATRIX_4X4_CLASS_HEADER