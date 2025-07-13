#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <stdexcept>


namespace mth
{
	template<std::size_t SIZE>
	class Vec
	{
	public:
		Vec();
		Vec(const Vec& another);
		Vec(std::initializer_list<float> init_list);

		std::size_t size() const;

		float operator[](std::size_t index) const;
		float& operator[](std::size_t index);

	protected:
		float values[SIZE];
	};
}


template<std::size_t SIZE>
mth::Vec<SIZE>::Vec()
{
	for (std::size_t i = 0; i < SIZE; i++)
		values[i] = 0;
}

template<std::size_t SIZE>
mth::Vec<SIZE>::Vec(const Vec& another)
{
	if (SIZE != another.size())
		throw std::invalid_argument("The lengths of the vectors must match");

	for (std::size_t i = 0; i < SIZE; i++)
		values[i] = another[i];
}

template<std::size_t SIZE>
mth::Vec<SIZE>::Vec(std::initializer_list<float> init_list)
{
	if (SIZE != init_list.size())
		throw std::invalid_argument("The lengths of the vectors must match");

	std::size_t i = 0;
	for (float value : init_list)
		values[i++] = value;
}


template<std::size_t SIZE>
std::size_t mth::Vec<SIZE>::size() const
{
	return SIZE;
}


template<std::size_t SIZE>
float mth::Vec<SIZE>::operator[](std::size_t index) const
{
	if (index >= SIZE)
		throw std::out_of_range("Vector");
	return values[index];
}

template<std::size_t SIZE>
float& mth::Vec<SIZE>::operator[](std::size_t index)
{
	if (index >= SIZE)
		throw std::out_of_range("Vector");
	return values[index];
}

#endif //VECTOR_HEADER