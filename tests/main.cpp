#include <iostream>

#include <Math/Vec.hpp>


void print(mth::Vec<3> vec)
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

	mth::Vec<3> v2 = {4, 9, 5};
	print(v2);

	mth::Vec<3> v3 = v2;
	print(v3);
	
	system("pause");
	return 0;
}