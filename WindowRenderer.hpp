#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "SortingElement.hpp"


#include <list>
#include <memory>

// 
using Ushort = unsigned short;

struct RectangleDimensions {
	double width;	// Width of a single element
	double height;	// Height of an element with value 1
};

struct WindowConfig {
	Ushort height = 1080;
	Ushort width = 1920;
	Ushort frames_per_second = 0; // Zero means no upper bound
	Ushort millisecond_delay = 0;
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
	void setMillisecondDelay(const Ushort ms) noexcept {
		millisecond_delay = ms;
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
	RectangleDimensions dimensions;


	void draw_rectangles();
	void draw_text(const sf::Time& time);
	void run_window();
public:

	WindowRenderer(const WindowConfig& config, const std::vector<Ushort>& list);

	void start();
	void step();
	void draw(const sf::Time& time);
	void create_rectangles(const std::vector<Ushort>& list);
};