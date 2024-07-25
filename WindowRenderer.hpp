#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <list>

using Ushort = unsigned short;

struct Dimensions {
	float width;
	float height;
};


struct WindowConfig {
	Ushort height = 900;
	Ushort width = 1600;
	Ushort frames_per_second = 0;
	Ushort microsecond_delay = 0;
	sf::String title = "Sorting Algorithm Visualizer";
	sf::Uint32 style = sf::Style::Default;
	bool vSync = false;

	void setFullscreen(const bool fullscreen = true) noexcept {
		if (fullscreen)
			style |= sf::Style::Fullscreen;
		else
			style &= ~sf::Style::Fullscreen;
	}
	void setScreenSize(const Ushort w, Ushort h) noexcept {
		width = w;
		height = h;
	}

	// Set the approximate FPS. A Value of 0 means no upper bound
	void setFramesPerSecond(const Ushort fps) noexcept {
		frames_per_second = fps;
	}
	// A more precise way to control FPS, makes the thread sleep
	void setMicrosecondDelay(const Ushort us) noexcept {
		microsecond_delay = us;
	}
	void setVSync(const bool vsync = true) noexcept {
		vSync = vsync;
	}
	void setTitle(const sf::String& t) noexcept {
		title = t;
	}
};

class WindowRenderer {
private:
	bool paused = false;
	sf::RenderWindow render_window;
	sf::Font text_font;
	sf::Text DEBUG_TEXT; // Temporary, used for time measurements
	std::vector<sf::RectangleShape> rectangles;
	sf::Clock last_draw_call_clock;
	Dimensions rectangle_dimensions;
	Dimensions window_dimensions;


	void draw_rectangles() noexcept;
	void draw_text();

	void create_rectangles(const std::vector<Ushort>& list);
public:

	WindowRenderer(const WindowConfig& config, const std::vector<Ushort>& list);

	void set_active(const bool active);
	void poll_event();
	void clear(const sf::Color c) noexcept;
	void close() noexcept;
	void display() noexcept;
	void step() noexcept;
	void draw() noexcept;

	// Swaps the rectangle positions on screen, and in the vector
	void swap(const unsigned int& idx1, const unsigned int& idx2);
	void set_value_at(const Ushort& new_value, const unsigned int& idx);
	void set_color_at(const unsigned long long idx, const sf::Color c);

	bool is_open() const noexcept;
	bool get_event(sf::Event& e);

	std::vector<sf::RectangleShape> get_rectangles() const;
};