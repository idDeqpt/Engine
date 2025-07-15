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

		const Row operator[](unsigned int index) const;
		Row operator[](unsigned int index);

	protected:
		float values[16];
	};
}

#endif //MATRIX_4X4_CLASS_HEADER