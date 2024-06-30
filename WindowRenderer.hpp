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
	
	void set_rectangle_data(const std::vector<int>& list);
public:

	WindowRenderer(const sf::VideoMode video_mode, const sf::String& title);

	void display() noexcept;
	void clear() noexcept;

	void create_rectangles(const std::vector<int>& list);
	void draw_rectangles() noexcept;

	void start(const std::vector<int>& list) noexcept;

};

