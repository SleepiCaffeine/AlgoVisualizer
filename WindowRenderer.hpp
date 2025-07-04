#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <list>
#include <chrono>

using Ushort = unsigned short;

struct Dimensions {
	float width;
	float height;
};


struct WindowConfig {

	Ushort height = 900;
	Ushort width = 1600;

	Ushort old_height = height;
	Ushort old_width = width;

	Ushort frames_per_second = 0;
	size_t millisecond_delay = 0;
	sf::String title = "Sorting Algorithm Visualizer";
	sf::Uint32 style = sf::Style::Default;
	bool vSync = false;
	bool add_outline = true;

	void setFullscreen(const bool fullscreen = true) noexcept {
		if (fullscreen) {
			style |= sf::Style::Fullscreen;
			height = 1080;
			width  = 1920;
		}
		else {
			height = old_height;
			width = old_width;
			style &= ~sf::Style::Fullscreen;
		}
	}
	void setScreenSize(const Ushort w, Ushort h) noexcept {
		old_width = width;
		old_height = height;

		width = w;
		height = h;
	}

	// Set the approximate FPS. A Value of 0 means no upper bound
	void setFramesPerSecond(const Ushort fps) noexcept {
		frames_per_second = fps;
	}
	// A more precise way to control FPS, makes the thread sleep
	void setMillisecondDelay(const size_t ms) noexcept {
		millisecond_delay = ms;
	}
	void setVSync(const bool vsync = true) noexcept {
		vSync = vsync;
	}
	void setTitle(const sf::String& t) noexcept {
		title = t;
	}

	void setOutline(const bool active = true) noexcept {
		add_outline = active;
	}
};

class WindowRenderer {
private:
	bool paused = false;
	sf::RenderWindow render_window;

	// On screen stuff
	sf::Font text_font;
	sf::Text timing_text;	// text that displays delay+fps+total
	sf::Text stats_text;	// text that displays reads+writes+swaps
	std::vector<sf::RectangleShape> rectangles;
	sf::Clock last_draw_call;

	// Extra text data
	size_t twrites{0};
	size_t treads{0};
	size_t tswaps{0};

	// Sounds
	sf::SoundBuffer sound_buf;
	std::list<sf::Sound> sounds;

	// Dimensions for reference
	Dimensions rectangle_dimensions;
	Dimensions window_dimensions;


	void draw_rectangles() noexcept;
	void draw_text();

	void create_rectangles(const std::vector<size_t>& list, const bool with_outline = true);
	void delete_sounds() noexcept;
public:

	WindowRenderer(const WindowConfig& config, const std::vector<size_t>& list);

	void set_active(const bool active);
	void poll_event();
	void clear(const sf::Color c) noexcept;
	void close() noexcept;
	void display() noexcept;
	void step() noexcept;
	void draw() noexcept;

	enum struct Statistic {READ, WRITE, SWAP, NONE};
	void increment_statistic(const Statistic& s) noexcept;

	// Swaps the rectangle positions on screen, and in the vector
	void swap(const size_t& idx1, const size_t& idx2);
	void set_value_at(const size_t& new_value, const size_t& idx);
	void set_color_at(const size_t idx, const sf::Color c);

	bool is_open() const noexcept;
	bool get_event(sf::Event& e);

	void add_sound(const size_t& value) noexcept;

	std::vector<sf::RectangleShape> get_rectangles() const;
};