#include <utility>
#include <cassert>
#include <cmath>


namespace eng::mth
{

namespace detail
{

constexpr bool near_zero(float x)
{
	return (x < 1e-12f) && (x > -1e-12f);
}

} //namespace detail

template<unsigned int N>
Mat<N>::ConstRow::ConstRow(const float* values):
	values(values)
{}

template<unsigned int N>
const float& mth::Mat<N>::ConstRow::operator[](unsigned int index) const
{
	assert(index < N);
	return values[index];
}


template<unsigned int N>
Mat<N>::Row::Row(float* values):
	values(values)
{}

template<unsigned int N>
float& mth::Mat<N>::Row::operator[](unsigned int index)
{
	assert(index < N);
	return values[index];
}



template<unsigned int N>
Mat<N>::Mat(float v)
{
	for (unsigned int i = 0; i < N*N; i++)
		values[i] = v;
}

template<unsigned int N>
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 1), int>>
Mat<N>::Mat(Args... args):
	values{static_cast<float>(args)...}
{
	static_assert(sizeof...(args) == N*N);
}


template<unsigned int N>
constexpr Mat<N> Mat<N>::getIdentity()
{
    Mat<N> res{};
    for (unsigned int i = 0; i < N*N; i += N + 1)
        res.values[i] = 1.0f;
    return res;
}


template<unsigned int N>
const typename Mat<N>::ConstRow Mat<N>::operator[](unsigned int index) const
{
	assert(index < N);
	return Mat<N>::ConstRow(&(values[index*N]));
}

template<unsigned int N>
typename Mat<N>::Row Mat<N>::operator[](unsigned int index)
{
	assert(index < N);
	return Mat<N>::Row(&(values[index*N]));
}



template<unsigned int N>
float det(const Mat<N>& mat)
{

	double a[N][N];
	for (unsigned int i = 0; i < N; i++)
		for (unsigned int j = 0; j < N; j++)
			a[i][j] = static_cast<double>(mat[i][j]);

	double d = 1.0;

	for (unsigned int i = 0; i < N; i++)
	{
		unsigned int pivot = i;
		for (unsigned int j = i + 1; j < N; j++)
			if (std::abs(a[j][i]) > std::abs(a[pivot][i]))
				pivot = j;

		if (detail::near_zero(a[pivot][i]))
			return 0.0f;

		if (pivot != i)
		{
			for (unsigned int k = i; k < N; k++)
				std::swap(a[i][k], a[pivot][k]);
			d = -d;
		}

		d *= a[i][i];

		for (unsigned int j = i + 1; j < N; j++)
		{
			double factor = a[j][i]/a[i][i];
			for (unsigned int k = i + 1; k < N; k++)
				a[j][k] -= factor*a[i][k];
		}
	}

	return static_cast<float>(d);
}

template<unsigned int N>
bool invert(const Mat<N>& in, Mat<N>& out)
{
	double aug[N][2*N] = {};
	for (unsigned int i = 0; i < N; i++)
	{
		for (unsigned int j = 0; j < N; j++)
			aug[i][j] = static_cast<double>(in[i][j]);
		aug[i][N + i] = 1.0;
	}

	for (unsigned int i = 0; i < N; i++)
	{
		unsigned int pivot = i;
		double max_val = std::abs(aug[i][i]);
		for (unsigned int r = i + 1; r < N; r++)
		{
			double val = std::abs(aug[r][i]);
			if (val > max_val)
			{
				max_val = val;
				pivot = r;
			}
		}

		if (detail::near_zero(max_val))
			return false;

		if (pivot != i)
			for (unsigned int c = i; c < 2*N; c++)
				std::swap(aug[i][c], aug[pivot][c]);

		double inv_diag = 1.0/aug[i][i];
		for (unsigned int c = i; c < 2*N; c++)
			aug[i][c] *= inv_diag;

		for (unsigned int r = 0; r < N; r++)
			if (r != i)
			{
				double factor = aug[r][i];
				if (factor != 0.0)
					for (unsigned int c = i; c < 2 * N; c++)
						aug[r][c] -= factor * aug[i][c];
			}
	}

	for (unsigned int i = 0; i < N; i++)
		for (unsigned int j = 0; j < N; j++)
			out[i][j] = static_cast<float>(aug[i][N + j]);

	return true;
}



inline float det(const Mat2& mat)
{
	return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];
}

inline float det(const Mat3& mat)
{
	float ac0;
	float ac1;
	float ac2;
	return det(mat, ac0, ac1, ac2);
}

inline float det(const Mat4& mat)
{
	float s0;
	float s1;
	float s2;
	float s3;
	float s4;
	float s5;

	float c5;
	float c4;
	float c3;
	float c2;
	float c1;
	float c0;

	return det(mat, s0, s1, s2, s3, s4, s5, c5, c4, c3, c2, c1, c0);
}

inline bool invert(const Mat2& in, Mat2& out)
{
	float det = mth::det(in);
	return invert(in, det, out);
}

inline bool invert(const Mat3& in, Mat3& out)
{
	float ac0;
	float ac1;
	float ac2;
	float det = mth::det(in, ac0, ac1, ac2);
	return invert(in, det, ac0, ac1, ac2, out);
}

inline bool invert(const Mat4& in, Mat4& out)
{
	float s0;
	float s1;
	float s2;
	float s3;
	float s4;
	float s5;

	float c5;
	float c4;
	float c3;
	float c2;
	float c1;
	float c0;

	float det = mth::det(in, s0, s1, s2, s3, s4, s5, c5, c4, c3, c2, c1, c0);
	return invert(in, det, s0, s1, s2, s3, s4, s5, c5, c4, c3, c2, c1, c0, out);
}


inline float det(const Mat3& mat, float& out_ac0, float& out_ac1, float& out_ac2)
{
	out_ac0 =    mat[1][1]*mat[2][2] - mat[1][2]*mat[2][1];
	out_ac1 = - (mat[1][0]*mat[2][2] - mat[1][2]*mat[2][0]);
	out_ac2 =    mat[1][0]*mat[2][1] - mat[1][1]*mat[2][0];
	return mat[0][0]*out_ac0 + mat[0][1]*out_ac1 + mat[0][2]*out_ac2;
}

inline float det(const Mat4& mat, float& out_s0, float& out_s1, float& out_s2, float& out_s3, float& out_s4, float& out_s5,
                                  float& out_c5, float& out_c4, float& out_c3, float& out_c2, float& out_c1, float& out_c0)
{
	out_s0 = mat[0][0]*mat[1][1] - mat[1][0]*mat[0][1];
	out_s1 = mat[0][0]*mat[1][2] - mat[1][0]*mat[0][2];
	out_s2 = mat[0][0]*mat[1][3] - mat[1][0]*mat[0][3];
	out_s3 = mat[0][1]*mat[1][2] - mat[1][1]*mat[0][2];
	out_s4 = mat[0][1]*mat[1][3] - mat[1][1]*mat[0][3];
	out_s5 = mat[0][2]*mat[1][3] - mat[1][2]*mat[0][3];

	out_c5 = mat[2][2]*mat[3][3] - mat[3][2]*mat[2][3];
	out_c4 = mat[2][1]*mat[3][3] - mat[3][1]*mat[2][3];
	out_c3 = mat[2][1]*mat[3][2] - mat[3][1]*mat[2][2];
	out_c2 = mat[2][0]*mat[3][3] - mat[3][0]*mat[2][3];
	out_c1 = mat[2][0]*mat[3][2] - mat[3][0]*mat[2][2];
	out_c0 = mat[2][0]*mat[3][1] - mat[3][0]*mat[2][1];

	return out_s0*out_c5 - out_s1*out_c4 + out_s2*out_c3 + out_s3*out_c2 - out_s4*out_c1 + out_s5*out_c0;
}

inline bool invert(const Mat2& in, float pre_comp_det, Mat2& out)
{
	if (detail::near_zero(pre_comp_det)) return false;

	float inv_det = 1.0f/pre_comp_det;

	out[0][0] =  in[1][1]*inv_det;
	out[0][1] = -in[0][1]*inv_det;
	out[1][0] = -in[1][0]*inv_det;
	out[1][1] =  in[0][0]*inv_det;

	return true;
}

inline bool invert(const Mat3& in, float pre_comp_det, float pre_comp_ac0, float pre_comp_ac1, float pre_comp_ac2, Mat3& out)
{
	if (detail::near_zero(pre_comp_det))
		return false;

	float inv_det = 1.0f/pre_comp_det;
	
	out[0][0] = pre_comp_ac0*inv_det;
	out[0][1] = -(in[0][1]*in[2][2] - in[0][2]*in[2][1])*inv_det;
	out[0][2] =  (in[0][1]*in[1][2] - in[0][2]*in[1][1])*inv_det;

	out[1][0] = pre_comp_ac1*inv_det;
	out[1][1] =  (in[0][0]*in[2][2] - in[0][2]*in[2][0])*inv_det;
	out[1][2] = -(in[0][0]*in[1][2] - in[0][2]*in[1][0])*inv_det;

	out[2][0] = pre_comp_ac2*inv_det;
	out[2][1] = -(in[0][0]*in[2][1] - in[0][1]*in[2][0])*inv_det;
	out[2][2] =  (in[0][0]*in[1][1] - in[0][1]*in[1][0])*inv_det;

	return true;
}

inline bool invert(const Mat4& in, float pre_comp_det, float pre_comp_s0, float pre_comp_s1, float pre_comp_s2, float pre_comp_s3, float pre_comp_s4, float pre_comp_s5,
                                                       float pre_comp_c5, float pre_comp_c4, float pre_comp_c3, float pre_comp_c2, float pre_comp_c1, float pre_comp_c0, Mat4& out)
{
	if (detail::near_zero(pre_comp_det))
		return false;

	float inv_det = 1.0f/pre_comp_det;

	out[0][0] = ( in[1][1]*pre_comp_c5 - in[1][2]*pre_comp_c4 + in[1][3]*pre_comp_c3)*inv_det;
	out[0][1] = (-in[0][1]*pre_comp_c5 + in[0][2]*pre_comp_c4 - in[0][3]*pre_comp_c3)*inv_det;
	out[0][2] = ( in[3][1]*pre_comp_s5 - in[3][2]*pre_comp_s4 + in[3][3]*pre_comp_s3)*inv_det;
	out[0][3] = (-in[2][1]*pre_comp_s5 + in[2][2]*pre_comp_s4 - in[2][3]*pre_comp_s3)*inv_det;

	out[1][0] = (-in[1][0]*pre_comp_c5 + in[1][2]*pre_comp_c2 - in[1][3]*pre_comp_c1)*inv_det;
	out[1][1] = ( in[0][0]*pre_comp_c5 - in[0][2]*pre_comp_c2 + in[0][3]*pre_comp_c1)*inv_det;
	out[1][2] = (-in[3][0]*pre_comp_s5 + in[3][2]*pre_comp_s2 - in[3][3]*pre_comp_s1)*inv_det;
	out[1][3] = ( in[2][0]*pre_comp_s5 - in[2][2]*pre_comp_s2 + in[2][3]*pre_comp_s1)*inv_det;

	out[2][0] = ( in[1][0]*pre_comp_c4 - in[1][1]*pre_comp_c2 + in[1][3]*pre_comp_c0)*inv_det;
	out[2][1] = (-in[0][0]*pre_comp_c4 + in[0][1]*pre_comp_c2 - in[0][3]*pre_comp_c0)*inv_det;
	out[2][2] = ( in[3][0]*pre_comp_s4 - in[3][1]*pre_comp_s2 + in[3][3]*pre_comp_s0)*inv_det;
	out[2][3] = (-in[2][0]*pre_comp_s4 + in[2][1]*pre_comp_s2 - in[2][3]*pre_comp_s0)*inv_det;

	out[3][0] = (-in[1][0]*pre_comp_c3 + in[1][1]*pre_comp_c1 - in[1][2]*pre_comp_c0)*inv_det;
	out[3][1] = ( in[0][0]*pre_comp_c3 - in[0][1]*pre_comp_c1 + in[0][2]*pre_comp_c0)*inv_det;
	out[3][2] = (-in[3][0]*pre_comp_s3 + in[3][1]*pre_comp_s1 - in[3][2]*pre_comp_s0)*inv_det;
	out[3][3] = ( in[2][0]*pre_comp_s3 - in[2][1]*pre_comp_s1 + in[2][2]*pre_comp_s0)*inv_det;

	return true;
}


template<unsigned int N>
Mat<N> operator*(const Mat<N>& a, const Mat<N>& b)
{
	Mat<N> r{};

	for (unsigned int i = 0; i < N; i++)
	{
		const float* a_row = a.values + i*N;
		float*       r_row = r.values + i*N;

		for (unsigned int k = 0; k < N; k++)
		{
			const float  a_ik  = a_row[k];
			const float* b_row = b.values + k*N;

			for (unsigned int j = 0; j < N; j++)
				r_row[j] += a_ik*b_row[j];
		}
	}
	return r;
}

template<unsigned int N>
Vec<N> operator*(const Mat<N>& a, const Vec<N>& v)
{
	Vec<N> r{};
	for (unsigned int i = 0; i < N; i++)
	{
		float sum = 0.0f;
		const float* a_row = a.values + i*N;
		for (unsigned int j = 0; j < N; j++)
			sum += a_row[j]*v[j];
		r[i] = sum;
	}
	return r;
}

} //namespace eng::mth