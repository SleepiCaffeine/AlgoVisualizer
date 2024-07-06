#include "SortingArray.hpp"
#include "WindowRenderer.hpp"
#include "Something.hpp"

#include <numeric> // std::iota

int main()
{
    WindowRenderer window_r(sf::VideoMode(1120, 630), 60);
    std::vector<int> arr(50);
    std::iota(arr.begin(), arr.end(), 1);
    window_r.initialize(arr);

    while (window_r.is_window_alive()) {
        window_r.step();
    }

    return 0;
}