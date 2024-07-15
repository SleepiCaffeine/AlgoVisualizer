#include "AlgorithmVisualizer.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>

int main()
{
    std::vector<int> arr(1500);
    std::iota(arr.begin(), arr.end(), 1);
    std::shuffle(arr.begin(), arr.end(), std::mt19937{std::random_device{}()});

    AlgorithmVisualizer av(sf::VideoMode(1920, 1080), "read_sound.ogg");
	av.set_delay(0);
    av.initialize_list(arr);
	av.set_sorting_method(AlgorithmVisualizer::SortingMethod::QuickSort);
	av.start();
    return 0;
}