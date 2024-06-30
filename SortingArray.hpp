#include <vector>

class SortingArray
{
public:
	explicit SortingArray(const std::initializer_list<int> list);
	// Public Data, since otherwise sorting would be annoying
	std::vector<int> elements;

	int get_min_value() const;
	int get_max_value() const;
};