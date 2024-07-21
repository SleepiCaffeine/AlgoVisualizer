#include "AlgorithmVisualizer.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>
#include <ranges>

int main()
{
    std::vector<unsigned short> arr(1000);
    std::iota(arr.begin(), arr.end(), 1);
    std::ranges::shuffle(arr, std::mt19937{std::random_device{}()});


    WindowConfig cfg;
    cfg.setFullscreen(false);
    WindowRenderer wr(cfg, arr);
}