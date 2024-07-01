#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <vector>

struct RectangleData {
	sf::Vector2f size;
	int max_value;
};


class WindowRenderer
{
private:
	sf::RenderWindow window;
	std::vector<sf::RectangleShape> rectangle_array;
	RectangleData rect_data;
	unsigned int delay_in_ms;
	
	void set_rectangle_data(const std::vector<int>& list);
public:

	WindowRenderer(const sf::VideoMode video_mode, const sf::String& title, const unsigned int& delay_in_ms);

	// Handling window processes
	void display() noexcept;
	void clear() noexcept;
	void start(const std::vector<int>& list) noexcept;

	// Handling text
	void generate_and_draw_text() noexcept;

	// Handling Rectangles
	void create_rectangles(const std::vector<int>& list);
	void swap_rectangle_positions(const int& idx1, const int& idx2) noexcept;
	void draw_rectangles() noexcept;

};

