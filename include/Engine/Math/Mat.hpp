#ifndef MATRIX_TEMPLATE_CLASS_HEADER
#define MATRIX_TEMPLATE_CLASS_HEADER

#include <Engine/Math/Vec.hpp>
#include <type_traits>

namespace eng::mth
{
	template<unsigned int N>
	struct Mat
	{
		static_assert(N >= 2);
		struct ConstRow
		{
			ConstRow(const float* values);
			const float& operator[](unsigned int index) const;
		private: const float* values;
		};
		struct Row
		{
			Row(float* values);
			float& operator[](unsigned int index);
		private: float* values;
		};

		float values[N*N] = {};

		Mat() = default;
		Mat(float v);

		template<typename... Args, std::enable_if_t<(sizeof...(Args) > 1), int> = 0>
		Mat(Args... args);

		Mat(const Mat& mat) = default;
		Mat(Mat&& mat) = default;

		static constexpr Mat getIdentity();

		Mat& operator=(const Mat& mat) = default;
		Mat& operator=(Mat&& mat) = default;

		const ConstRow operator[](unsigned int index) const;
		Row operator[](unsigned int index);
	};

	using Mat2 = Mat<2>;
	using Mat3 = Mat<3>;
	using Mat4 = Mat<4>;

	template<unsigned int N>  float det(const Mat<N>& mat);
	template<unsigned int N>  bool invert(const Mat<N>& in, Mat<N>& out);

	inline float det(const Mat2& mat);
	inline float det(const Mat3& mat);
	inline float det(const Mat4& mat);
	inline bool invert(const Mat2& in, Mat2& out);
	inline bool invert(const Mat3& in, Mat3& out);
	inline bool invert(const Mat4& in, Mat4& out);

	inline float det(const Mat3& mat, float& out_ac0, float& out_ac1, float& out_ac2);
	inline float det(const Mat4& mat, float& out_s0, float& out_s1, float& out_s2, float& out_s3, float& out_s4, float& out_s5,
	                                  float& out_c5, float& out_c4, float& out_c3, float& out_c2, float& out_c1, float& out_c0);
	inline bool invert(const Mat2& in, float pre_comp_det, Mat2& out);
	inline bool invert(const Mat3& in, float pre_comp_det, float pre_comp_ac0, float pre_comp_ac1, float pre_comp_ac2, Mat3& out);
	inline bool invert(const Mat4& in, float pre_comp_det, float pre_comp_s0, float pre_comp_s1, float pre_comp_s2, float pre_comp_s3, float pre_comp_s4, float pre_comp_s5,
	                                                       float pre_comp_c5, float pre_comp_c4, float pre_comp_c3, float pre_comp_c2, float pre_comp_c1, float pre_comp_c0, Mat4& out);

	template<unsigned int N>  Mat<N> operator*(const Mat<N>& a, const Mat<N>& b);
	template<unsigned int N>  Vec<N> operator*(const Mat<N>& a, const Vec<N>& b);
}

#include <Engine/Math/Mat.inl>

#endif //MATRIX_TEMPLATE_CLASS_HEADER