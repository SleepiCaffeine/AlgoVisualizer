#include "VisualArray.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>
#include <ranges>
#include <thread>
#include <utility>

using ULL = unsigned long long;
bool can_draw = true;


void bubblesort(VisualArray& va) {
    va.get_wPtr().get()->set_active(true);

    for (int i = 0; i < va.size() - 1; ++i) {
        for (int j = 0; j < va.size() - 1 - i; ++j) {
            if (va.get_at(j) > va.get_at(j + 1)) {
                va.swap(j, j + 1);
            }

            va.get_wPtr().get()->clear(sf::Color::Black);
            va.get_wPtr().get()->step();
            va.get_wPtr().get()->display();
        }
    }

    can_draw = true;
}

int main()
{
    std::vector<unsigned short> arr(1000);
    std::iota(arr.begin(), arr.end(), 1);
    std::ranges::shuffle(arr, std::mt19937{std::random_device{}()});
    
    auto wr = std::make_shared<WindowRenderer>(WindowConfig(), arr);
    VisualArray vis_array(wr, 0);
    vis_array.copy(arr);


    wr.get()->set_active(false);
    sf::Thread t(bubblesort, std::ref(vis_array));
    t.launch();

    can_draw = false;

    while (wr.get()->is_open()) {
        sf::Event e;
        while (wr.get()->get_event(e)) {
            
            if (e.type == sf::Event::Closed) {
                wr.get()->close();
            }

            else if (e.type == sf::Event::KeyPressed &&
                e.key.scancode == sf::Keyboard::Scancode::Escape) {
                wr.get()->close();
            }
        
        }
        
        if (can_draw) {
            wr.get()->clear(sf::Color::Black);
            wr.get()->step();
            wr.get()->display();
        }

    
    }

}