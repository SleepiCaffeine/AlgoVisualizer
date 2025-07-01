#include "WindowRenderer.hpp"
#include <format>
#include <stdexcept>
#include <algorithm>


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
	// STATS_TEXT
	stats_text.setString("Reads: " + std::to_string(treads) +
		"\nWrites: " + std::to_string(twrites) +
		"\nSwaps: " + std::to_string(tswaps));

	render_window.draw(stats_text);


	// TIMING_TEXT
	static size_t      ms_total = 0;

	// Please God explain, why does the whole code break
	// If this SPECIFIC VARIABLE ISN'T A FLOAT?!
	// God has forsaken us, forgotten us, left us to rot
	const float ms_in_1s = 1000.f;

	const unsigned long time_between_calls = last_draw_call.getElapsedTime().asMilliseconds();
	
	// There are some cases when time_between_calls is 0, hence a division by 0 error
	const unsigned int FPS = (time_between_calls) ? ms_in_1s / time_between_calls : ms_in_1s / 1;
	ms_total += time_between_calls;

	// Setting the string in such an ugly way shaved off about 40% of time between draw calls...
	timing_text.setString("Delay: " + std::to_string(time_between_calls) +
		" ms\nFPS: " + std::to_string(FPS) +
		"\nTotal Time: " + std::to_string(ms_total / 1000) + "s");
	render_window.draw(timing_text);
}


bool WindowRenderer::get_event(sf::Event& e) {
	return render_window.pollEvent(e);
}



#define MAX_SOUNDS 150
void WindowRenderer::add_sound(const size_t& value) noexcept
{
	const float MIN_PITCH = 1e-5;
	static size_t counter = 0;

	float approx_max_element  = window_dimensions.height / rectangle_dimensions.height;
	float normalized_position = value / approx_max_element;
	float multiplied_position = normalized_position * 2;

	multiplied_position = std::max(multiplied_position, MIN_PITCH);

	auto iter = sounds.begin();
	std::advance(iter, counter);

	iter->stop();
	iter->setPitch(multiplied_position);
	iter->play();

	++counter;
	counter %= MAX_SOUNDS;
}

void WindowRenderer::poll_event() {
	sf::Event event;
	while (render_window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			render_window.close();
			delete_sounds();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				render_window.close();
				delete_sounds();
			}
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

#define SOUND_VOLUME 10
WindowRenderer::WindowRenderer(const WindowConfig& cfg, const std::vector<size_t>& list)
	: render_window(sf::VideoMode(cfg.width, cfg.height), cfg.title, cfg.style),
	  window_dimensions{to<float>(render_window.getSize().x), to<float>(render_window.getSize().y)} {

	// Window configurations
	render_window.setVerticalSyncEnabled(cfg.vSync);
	render_window.setFramerateLimit(cfg.frames_per_second);

	last_draw_call.restart();

	if (cfg.millisecond_delay && !cfg.frames_per_second) {
		// 1000ms in 1s
		render_window.setFramerateLimit( 1000 / cfg.millisecond_delay);
	}


	// Audio
	if (!sound_buf.loadFromFile("read_sound2.ogg")) {
		throw std::runtime_error("Failed to load audio file");
	}
	sf::Sound sound(sound_buf);
	sound.setVolume(SOUND_VOLUME);
	sounds = std::list<sf::Sound>(MAX_SOUNDS, sound);


	// Rectangle Dimensions
	size_t max_element = *std::max_element(list.begin(), list.end());
	// In theory - list.size() and max_element should be the same, however I do not trust people.
	rectangle_dimensions.width  = window_dimensions.width  / to<float>(list.size());
	rectangle_dimensions.height = window_dimensions.height / to<float>(max_element);
	
	

	// Text stuff
	if (!text_font.loadFromFile("Minecraft.ttf")) {
		throw std::system_error::exception("Failed to load font");
	}
	

	timing_text.setFont(text_font);
	timing_text.setCharacterSize(16);
	timing_text.setFillColor(sf::Color::White);
	timing_text.setStyle(sf::Text::Bold);
	timing_text.setOutlineThickness(2);
	timing_text.setPosition(20, 20);

	stats_text = sf::Text(timing_text);
	stats_text.setPosition(180, 20);


	render_window.setActive(true);
	create_rectangles(list, cfg.add_outline);
}

void WindowRenderer::draw() noexcept
{
	draw_rectangles();
	draw_text();
	last_draw_call.restart();
}

void WindowRenderer::increment_statistic(const Statistic& s) noexcept
{
	switch (s) {
	case Statistic::READ:
		++treads;
		break;
	case Statistic::WRITE:
		++twrites;
		break;
	case Statistic::SWAP:
		++tswaps;
		break;
	default:
		break;
	}
}

void WindowRenderer::swap(const size_t& idx1, const size_t& idx2)
{
	const float x1 = rectangles.at(idx1).getPosition().x;
	const float x2 = rectangles.at(idx2).getPosition().x;

	rectangles.at(idx1).move(x2 - x1, 0);
	rectangles.at(idx2).move(x1 - x2, 0);

	std::swap(rectangles.at(idx1), rectangles.at(idx2));
}

void WindowRenderer::set_value_at(const size_t& new_value, const size_t& idx)
{
	float new_height = to<float>(new_value) * rectangle_dimensions.height;
	float new_ypos   = window_dimensions.height - new_height;
	float width      = rectangle_dimensions.width;

	rectangles.at(idx).setPosition(width * idx, new_ypos);
	rectangles.at(idx).setSize({ width, new_height });
}

void WindowRenderer::set_color_at(const size_t idx, const sf::Color c)
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

void WindowRenderer::create_rectangles(const std::vector<size_t>& list, const bool with_outline) {
	rectangles.clear();
	for (const size_t& element : list) {

		sf::RectangleShape shape;
		float height = to<float>(rectangle_dimensions.height * element);
		float x_pos = to<float>(rectangles.size() * rectangle_dimensions.width);
		float y_pos = window_dimensions.height - height;


		shape.setSize({ to<float>(rectangle_dimensions.width), height });
		shape.setFillColor(sf::Color::White);
		shape.setPosition(x_pos, y_pos);
		if (with_outline) {
			shape.setOutlineColor(sf::Color::Black);
			shape.setOutlineThickness(1);
		}
		rectangles.emplace_back(shape);
	}
}

void WindowRenderer::delete_sounds() noexcept
{
	for (int i = 0; i < sounds.size(); ++i) {
		sounds.front().setVolume(0);
		sounds.front().stop();
		sounds.erase(sounds.begin());
	}
	sounds.clear();
}
