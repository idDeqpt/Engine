#include <iostream>

#include <Engine/Math/Vec4.hpp>
#include <Engine/Math/Mat4.hpp>


void print(mth::Vec4& vec)
{
	std:: cout << "Vec4: ";
	for (unsigned int i = 0; i < 4; i++)
		std::cout << vec[i] << " ";
	std::cout << "End Vec4" << std::endl;
}

void print(mth::Mat4& mat)
{
	std:: cout << "Mat4: \n";
	for (unsigned int i = 0; i < 4; i++)
	{
		std::cout << "\t";
		for (unsigned int j = 0; j < 4; j++)
			std::cout << mat[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << "End Mat4" << std::endl;
}


int main()
{
	std::cout << "START\n";
	mth::Vec4 v1;
	print(v1);

	mth::Vec4 v2(7);
	print(v2);

	mth::Vec4 v3(8, 2, 3, 9);
	print(v3);

	mth::Vec4 v4 = v3;
	print(v4);

	v4.x = 67;
	std::cout << "v3 X: " << v4.x << std::endl;
	std::cout << "v2 X: " << v3.x << std::endl << std::endl;

	mth::Mat4 m1;
	print(m1);

	mth::Mat4 m2(9);
	print(m2);

	mth::Mat4 m3 = m2;
	print(m3);

	m3[2][1] = 16;
	print(m2);
	print(m3);
	
	system("pause");
	return 0;
}