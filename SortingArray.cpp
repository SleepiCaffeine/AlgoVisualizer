#include "SortingArray.hpp"


SortingArray::SortingArray(const std::initializer_list<int> list)
	: elements{ list }
{ }
 
int SortingArray::get_min_value() const
{
	int min_value = elements.at(0);

	for (const int& e : elements) {
		if (e < min_value)
			min_value = e;
	}

	return min_value;
}

int SortingArray::get_max_value() const
{
	int max_value = elements.at(0);

	for (const int& e : elements) {
		if (e > max_value)
			max_value = e;
	}

	return max_value;
}

