#include "AlgorithmVisualizer.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>
#include <ranges>

using ULL = unsigned long long;

int main()
{
    std::vector<unsigned short> arr(100);
    std::iota(arr.begin(), arr.end(), 1);
    std::ranges::shuffle(arr, std::mt19937{std::random_device{}()});


    /*AlgorithmVisualizer av(arr);
    while (true)
	{
		av.test_read();
	}*/

    WindowRenderer wr(WindowConfig(), arr);
}