#include "SortingArray.hpp"
#include "WindowRenderer.hpp"
#include "Something.hpp"

int main()
{
    WindowRenderer window_r(sf::VideoMode(1120, 630), "THing");
    std::vector<int> arr{ 1,2,3,4,6,5 };
    window_r.start(arr);
    return 0;
}