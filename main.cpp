#include "AlgorithmVisualizer.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>
#include <ranges>
#include <thread>
#include <utility>

bool can_draw = true;
void bubblesort(AlgorithmVisualizer& av) {
    av.set_active(true);
    for (int i = 0; i < av.size() - 1; ++i) {
        for (int j = 0; j < av.size() - 1 - i; ++j) {
            if (av.get_at(j) > av.get_at(j + 1)) {
                av.swap(j, j + 1);
            }
            av.clear(sf::Color::Black);
            av.step();
            av.display();
        }
    }

    can_draw = true;
}

using ULL = unsigned long long;



int main()
{
    std::vector<unsigned short> arr(1000);
    std::iota(arr.begin(), arr.end(), 1);
    std::ranges::shuffle(arr, std::mt19937{std::random_device{}()});


    AlgorithmVisualizer av(arr);

    av.set_active(false);
    can_draw = false;
    std::jthread j(bubblesort, std::ref(av));

    while (av.is_window_alive()) {

        // Event loop
        sf::Event e;
        while (av.get_event(e)) {
            switch (e.type) {
            case sf::Event::Closed:
                av.close();
                break;
            case sf::Event::KeyPressed:
                if (e.key.code == sf::Keyboard::Escape)
                    av.close();
                break;
            }
        }

        if (can_draw) {
            av.clear(sf::Color::Black);
            av.step();
            av.display();
        }

    }

}