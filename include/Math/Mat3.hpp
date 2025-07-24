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

		float det() const;

		float* getValuesPtr();

		const Row operator[](unsigned int index) const;
		Row operator[](unsigned int index);

	protected:
		float values[9];
	};
}

#endif //MATRIX_3X3_CLASS_HEADER