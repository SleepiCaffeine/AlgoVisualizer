#include "WindowRenderer.hpp"
#include <format>
#include <stdexcept>

// Saw this in a "Pezzza's Work" video, thought it looked nice
template <typename T>
[[nodiscard]] T to(const auto& value) noexcept { return static_cast<T>(value); }

void WindowRenderer::draw_rectangles() noexcept
{
	for (sf::RectangleShape& rect : rectangles) {
		render_window.draw(rect);
		rect.setFillColor(sf::Color::White);
	}
}

void WindowRenderer::draw_text()
{
	// Setting the string in such an ugly way shaved off about 40% of time between draw calls...
	// Delay: _ microseconds | FPS : 1 million / microseconds
	const float million = 1000000.f;
	auto time = last_draw_call_clock.getElapsedTime();
	DEBUG_TEXT.setString("Delay: "    + std::to_string(time.asMicroseconds()) +
						 " us\nFPS: " + std::to_string(to<int>(million / time.asMicroseconds())));
	render_window.draw(DEBUG_TEXT);
}


bool WindowRenderer::get_event(sf::Event& e) {
	return render_window.pollEvent(e);
}

void WindowRenderer::poll_event() {
	sf::Event event;
	while (render_window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			render_window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				render_window.close();

			if (event.key.code == sf::Keyboard::Space) {
				paused = !paused;
			}
			break;
		default:
			break;
		}
	}
}

void WindowRenderer::clear(const sf::Color c) noexcept
{
	render_window.clear(c);
}

void WindowRenderer::close() noexcept
{
	render_window.close();
}

void WindowRenderer::display() noexcept
{
	render_window.display();
}

void WindowRenderer::set_active(const bool active)
{
	render_window.setActive(active);
}

void WindowRenderer::step() noexcept
{
	if (!paused) {
		clear(sf::Color::Black);
		draw();
		display();
	}
}

WindowRenderer::WindowRenderer(const WindowConfig& cfg, const std::vector<Ushort>& list)
	: render_window(sf::VideoMode(cfg.width, cfg.height), cfg.title, cfg.style),
	  window_dimensions{to<float>(render_window.getSize().x), to<float>(render_window.getSize().y)} {

	render_window.setVerticalSyncEnabled(cfg.vSync);
	render_window.setFramerateLimit(cfg.frames_per_second);

	last_draw_call_clock.restart();

	if (cfg.microsecond_delay && !cfg.frames_per_second) {
		// 1000 us in 1ms ; 1000ms in 1s
		render_window.setFramerateLimit(1000 * 1000 / cfg.microsecond_delay);
	}


	Ushort max_element = *std::ranges::max_element(list.begin(), list.end());
	// In theory - list.size() and max_element should be the same, however I do not trust people.
	rectangle_dimensions.width  = window_dimensions.width  / to<float>(list.size());
	rectangle_dimensions.height = window_dimensions.height / to<float>(max_element);
	
	
	if (!text_font.loadFromFile("Minecraft.ttf")) {
		throw std::runtime_error("Failed to load font");
	}
	

	DEBUG_TEXT.setFont(text_font);
	DEBUG_TEXT.setCharacterSize(16);
	DEBUG_TEXT.setFillColor(sf::Color::White);
	DEBUG_TEXT.setStyle(sf::Text::Bold);
	DEBUG_TEXT.setOutlineThickness(2);
	DEBUG_TEXT.setPosition(20, 20);

	render_window.setActive(true);
	create_rectangles(list);
}

void WindowRenderer::draw() noexcept
{
	draw_rectangles();
	draw_text();
	last_draw_call_clock.restart();
}

void WindowRenderer::swap(const unsigned int& idx1, const unsigned int& idx2)
{
	const float x1 = rectangles.at(idx1).getPosition().x;
	const float x2 = rectangles.at(idx2).getPosition().x;

	rectangles.at(idx1).move(x2 - x1, 0);
	rectangles.at(idx2).move(x1 - x2, 0);

	std::swap(rectangles.at(idx1), rectangles.at(idx2));
}

void WindowRenderer::set_value_at(const Ushort& new_value, const unsigned int& idx)
{
	float new_height = to<float>(new_value) * rectangle_dimensions.height;
	float new_ypos   = window_dimensions.height - new_height;
	float width      = rectangle_dimensions.width;

	rectangles.at(idx).setPosition(width * idx, new_ypos);
	rectangles.at(idx).setSize({ width, new_height });
}

void WindowRenderer::set_color_at(const unsigned long long idx, const sf::Color c)
{
	rectangles.at(idx).setFillColor(c);
}

bool WindowRenderer::is_open() const noexcept
{
	return render_window.isOpen();
}

std::vector<sf::RectangleShape> WindowRenderer::get_rectangles() const
{
	return rectangles;
}

void WindowRenderer::create_rectangles(const std::vector<Ushort>& list) {
	rectangles.clear();
	for (const Ushort& element : list) {

		sf::RectangleShape shape;
		float height = to<float>(rectangle_dimensions.height * element);
		float x_pos = to<float>(rectangles.size() * rectangle_dimensions.width);
		float y_pos = window_dimensions.height - height;


		shape.setSize({ to<float>(rectangle_dimensions.width), height });
		shape.setFillColor(sf::Color::White);
		shape.setPosition(x_pos, y_pos);
		rectangles.emplace_back(shape);
	}
}
