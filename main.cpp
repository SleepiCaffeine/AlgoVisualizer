#include "AlgorithmVisualizer.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>

int main()
{

    // Thing t(size, audio_file);
    // t.set_delay(16);
    // t.initialize(list);
    // t.set_sorting_methoid(SortingMethod::BubbleSort);
    // t.start();

    std::vector<int> arr(50);
    std::iota(arr.begin(), arr.end(), 1);
    std::shuffle(arr.begin(), arr.end(), std::mt19937{std::random_device{}()});



    AlgorithmVisualizer av(sf::VideoMode(1120, 630), "read_sound.ogg");
	av.set_delay(500);
    av.initialize_list(arr);
	av.set_sorting_method(AlgorithmVisualizer::SortingMethod::BubbleSort);
	av.start();

    /*WindowRenderer window_r(sf::VideoMode(1120, 630), 60);
    std::vector<int> arr(50);
    std::iota(arr.begin(), arr.end(), 1);
    window_r.initialize(arr);

    while (window_r.is_window_alive()) {
        window_r.step();
    }*/

    return 0;
}