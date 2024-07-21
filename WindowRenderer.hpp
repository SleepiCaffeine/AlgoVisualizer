#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "SortingElement.hpp"


#include <list>
#include <memory>

// 
using Ushort = unsigned short;

struct Dimensions {
	double width;
	double height;
};


struct WindowConfig {
	Ushort height = 1080;
	Ushort width = 1920;
	Ushort frames_per_second = 1000; // Zero means no upper bound
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
	void setFramesPerSecond(const Ushort fps) noexcept {
		frames_per_second = fps;
	}
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
	sf::RenderWindow render_window;
	std::unique_ptr<sf::Text> DEBUG_TEXT; // Temporary, used for time measurements
	std::vector<sf::RectangleShape> rectangles;
	sf::Clock last_draw_call_clock;


	void draw_rectangles();
	void draw_text(const sf::Time& time);
	void run_window();

	void create_rectangles(const std::vector<Ushort>& list);
public:

	Dimensions rectangle_dimensions;
	Dimensions window_dimensions;

	WindowRenderer(const WindowConfig& config, const std::vector<Ushort>& list);

	void start();
	void poll_event();
	void step();
	void draw(const sf::Time& time);

	// Swaps the rectangle positions on screen, and in the vector
	void swap(const unsigned int& idx1, const unsigned int& idx2);
	void set_value_at(const Ushort& new_value, const unsigned int& idx);
	void set_color_at(const unsigned long long idx, const sf::Color c);

	bool is_open() const noexcept;

	std::vector<sf::RectangleShape> get_rectangles() const;
};