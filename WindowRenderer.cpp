#include "WindowRenderer.hpp"
#include <iostream>

// Saw this in a "Pezzza's Work" video, thought it looked nice
template <typename T>
[[no_discard]] T to(const auto& value) noexcept { return static_cast<T>(value); }


void WindowRenderer::draw_rectangles()
{
	for (const sf::RectangleShape& rect : rectangles) {
		render_window.draw(rect);
	}
}

void WindowRenderer::draw_text(const sf::Time& time)
{
	sf::String str = "Delay: " + std::to_string(time.asMicroseconds()) + " us";
	DEBUG_TEXT->setString(str);
	render_window.draw(*DEBUG_TEXT);
}

void run_window(sf::RenderWindow* render_window) {
	render_window->setActive(true);
	while (render_window->isOpen()) {

		render_window->display();
		render_window->clear(sf::Color::Black);
	}
}




void WindowRenderer::start(const std::vector<Ushort>& list) {
	//run_window(&render_window);
	while (render_window.isOpen()) {
		sf::Event event;
		while (render_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				render_window.close();
			}
		}
		render_window.clear(sf::Color::Black);
		step(list);
		render_window.display();
	}
}

void WindowRenderer::step(const std::vector<Ushort>& list)
{
	sf::Clock clock;
	draw(clock.getElapsedTime());
}

WindowRenderer::WindowRenderer(const WindowConfig& cfg, const std::vector<Ushort>& list)
	: render_window(sf::VideoMode(cfg.width, cfg.height), cfg.title, cfg.style) {

	Ushort max_element = *std::ranges::max_element(list.begin(), list.end());
	// In theory - list.size() and max_element should be the same, however I do not trust people.
	dimensions.width  = render_window.getSize().x / to<float>(list.size());
	dimensions.height = render_window.getSize().y / to<float>(max_element);



	DEBUG_TEXT = std::make_unique<sf::Text>();
	
	sf::Font font;
	
	if (!font.loadFromFile("Minecraft.ttf")) {
		throw std::runtime_error("Failed to load font");
	}
	

	DEBUG_TEXT->setFont(font);
	DEBUG_TEXT->setCharacterSize(16);
	DEBUG_TEXT->setFillColor(sf::Color::White);
	DEBUG_TEXT->setStyle(sf::Text::Bold);
	DEBUG_TEXT->setOutlineThickness(2);
	DEBUG_TEXT->setPosition(20, 20);

	create_rectangles(list);
	start(list);
}

void WindowRenderer::draw(const sf::Time& time)
{
	draw_rectangles();
	draw_text(time);
}

void WindowRenderer::create_rectangles(const std::vector<Ushort>& list) {
	rectangles.clear();
	for (const Ushort& element : list) {

		sf::RectangleShape shape;
		float height = to<float>(dimensions.height * element);
		float x_pos = to<float>(rectangles.size() * dimensions.width);
		float y_pos = render_window.getSize().y - height;


		shape.setSize({ to<float>(dimensions.width), height });
		shape.setFillColor(sf::Color::White);
		shape.setPosition(x_pos, y_pos);
		rectangles.emplace_back(shape);
	}
}
