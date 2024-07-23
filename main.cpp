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


    AlgorithmVisualizer av(arr);

    int counter = 0;
    while (true) {
        auto r  = av.get_at(counter);
        av.set_at(counter, 2);
        counter %= arr.size();
    }

}