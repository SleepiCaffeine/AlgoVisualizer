#include "WindowRenderer.hpp"
#include <format>

// Saw this in a "Pezzza's Work" video, thought it looked nice
template <typename T>
[[no_discard]] T to(const auto& value) noexcept { return static_cast<T>(value); }

sf::Mutex mutex;
void WindowRenderer::draw_rectangles()
{
	sf::Lock lock(mutex);
	for (const sf::RectangleShape& rect : rectangles) {
		render_window.draw(rect);
	}
}

void WindowRenderer::draw_text(const sf::Time& time)
{
	sf::Lock lock(mutex);
	// Setting the string in such an ugly way shaved off about 40% of time between draw calls...
	// Delay: _ microseconds | FPS : 1 million / microseconds
	DEBUG_TEXT->setString("Delay: "    + std::to_string(time.asMicroseconds()) +
						  " us\nFPS: " + std::to_string(to<unsigned short>(1000000.f / time.asMicroseconds())));
	render_window.draw(*DEBUG_TEXT);
}


void WindowRenderer::run_window() {
	sf::Lock lock(mutex);
	render_window.setActive(true);

	while (render_window.isOpen()) {
		render_window.clear(sf::Color::Black);
		step();
		render_window.display();
	}
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
			break;
		default:
			break;
		}
	}
}


void WindowRenderer::start() {
	render_window.setActive(false);
	sf::Thread t(&WindowRenderer::run_window, this);
	t.launch();


}

void WindowRenderer::step()
{
	rectangles.at(9).setFillColor(sf::Color::Green);
	draw(last_draw_call_clock.getElapsedTime());
	last_draw_call_clock.restart();
}

WindowRenderer::WindowRenderer(const WindowConfig& cfg, const std::vector<Ushort>& list)
	: render_window(sf::VideoMode(cfg.width, cfg.height), cfg.title, cfg.style),
	  window_dimensions{to<double>(render_window.getSize().x), to<double>(render_window.getSize().y)} {

	render_window.setVerticalSyncEnabled(cfg.vSync);
	render_window.setFramerateLimit(cfg.frames_per_second);

	if (cfg.microsecond_delay && !cfg.frames_per_second) {
		render_window.setFramerateLimit(1000000 / cfg.microsecond_delay);
	}


	Ushort max_element = *std::ranges::max_element(list.begin(), list.end());
	// In theory - list.size() and max_element should be the same, however I do not trust people.
	rectangle_dimensions.width  = window_dimensions.width  / to<float>(list.size());
	rectangle_dimensions.height = window_dimensions.height / to<float>(max_element);



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
	start();
}

void WindowRenderer::draw(const sf::Time& time)
{
	draw_rectangles();
	draw_text(time);
}

void WindowRenderer::swap(const unsigned int& idx1, const unsigned int& idx2)
{
	const float x1 = rectangles.at(idx1).getPosition().x;
	const float x2 = rectangles.at(idx2).getPosition().x;

	rectangles.at(idx1).move(x2 - x1, 0);
	rectangles.at(idx2).move(x1 - x2, 0);
}

void WindowRenderer::set_value_at(const Ushort& new_value, const unsigned int& idx)
{
	float new_height = new_value * rectangle_dimensions.height;
	float width = rectangle_dimensions.width;
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
