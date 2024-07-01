#include "WindowRenderer.hpp"



WindowRenderer::WindowRenderer(const sf::VideoMode video_mode, const sf::String& title, const unsigned int& delay_in_ms)
	: window{ video_mode, title }, rect_data{ sf::Vector2f(0,0), -1 }, delay_in_ms{delay_in_ms}
{ 
	window.setActive();
	window.setFramerateLimit(1000 / delay_in_ms);
}

void WindowRenderer::display() noexcept
{
	window.display();
}

void WindowRenderer::clear() noexcept
{
	window.clear(sf::Color::Black);
}

void WindowRenderer::draw_rectangles() noexcept
{
	for (const auto& rect : rectangle_array) {
		window.draw(rect);
	}
}

void WindowRenderer::start(const std::vector<int>& list) noexcept
{	
	// Generate the rectangles
	set_rectangle_data(list);
	create_rectangles(list);

	while (window.isOpen()) {

		// Event-Loop to keep things working
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}

		generate_and_draw_text();
		draw_rectangles();
		display();
		clear();
	}
}

void WindowRenderer::generate_and_draw_text() noexcept
{
	auto delay = sf::Text();
	auto font = sf::Font();
	font.loadFromFile("minecraft.ttf");
	delay.setFont(font);

	delay.setPosition(0, 0);
	delay.setString("Delay: " + std::to_string(delay_in_ms) + "ms");
	delay.setCharacterSize(20);
	delay.setFillColor(sf::Color::White);
	window.draw(delay);
}

// std::max_element implementation without needing <algorithm>
template<class InputIt>
static constexpr InputIt list_max(InputIt begin, InputIt end) {
	end = end - 1;
	if (begin == end)
		return end;

	InputIt max = begin;
	while (++begin != end)
		if (*begin > *max)
			max = begin;
	return max;
}


void WindowRenderer::set_rectangle_data(const std::vector<int>& list) {
	rect_data.max_value = *list_max(list.begin(), list.end());
	rect_data.size.x = static_cast<float>(window.getSize().x / list.size());
	rect_data.size.y = 1 / static_cast<float>(rect_data.max_value);
}

void WindowRenderer::create_rectangles(const std::vector<int>& list)
{
	rectangle_array.clear();
	for (const int& elem : list) {
		sf::RectangleShape rect;

		// How much of the window height is occupied by the rectangle
		float height     = rect_data.size.y * elem * window.getSize().y;
		// Where is the rectangle's x origin point
		float x_position = rectangle_array.size() * rect_data.size.x;

		rect.setPosition(x_position, window.getSize().y - height);
		rect.setSize(sf::Vector2f(rect_data.size.x, height));
	
		rectangle_array.push_back(rect);
	}
}

void WindowRenderer::swap_rectangle_positions(const int& idx1, const int& idx2) noexcept
{
	if (idx1 == idx2)
		return;

	const int diff = idx2 - idx1;
	rectangle_array[idx1].move(diff * rect_data.size.x, 0);
	rectangle_array[idx2].move(-diff * rect_data.size.x, 0);
}
