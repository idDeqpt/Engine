#include <iostream>

#include <Math/Vec.hpp>


void print(mth::Vec3 vec)
{
	std:: cout << "Vec: ";
	for (unsigned int i = 0; i < vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << std::endl;
}


int main()
{
	mth::Vec<3> v1;
	print(v1);

	mth::Vec3 v2 = {4, 9, 5};
	print(v2);

	mth::Vec3 v3 = v2;
	print(v3);

	v3.x = 67;
	std::cout << "v3 X: " << v3.x << std::endl;
	std::cout << "v2 X: " << v2.x << std::endl;
	
	system("pause");
	return 0;
}