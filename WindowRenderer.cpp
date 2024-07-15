#include "WindowRenderer.hpp"
#include <iostream>
#define MAX_SOUNDS 120
#define SOUND_VOLUME 20
#define MINIMUM_PITCH 0.5


WindowRenderer::WindowRenderer(const sf::VideoMode video_mode, const unsigned int& delay_in_ms, const sf::String& title, const sf::String& audio_file_name)
	: sound_array{ MAX_SOUNDS }, window{ video_mode, title }, rect_data{ sf::Vector2f(0,0), -1 }, delay_in_ms{ delay_in_ms }
{ 
	if (!SOUND_BUFFER.loadFromFile(audio_file_name)) {
		std::cout << "Could not load audio file" << std::endl;
	}

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
	for (auto& r : rectangle_array) {
		window.draw(r.get_rect());
		r.set_color(sf::Color::White);	// Reset Color
	}
}


void WindowRenderer::set_title(const sf::String& title) noexcept
{
	window.setTitle(title);
}

bool WindowRenderer::set_audio_file(const sf::String& audio_file) noexcept
{
	if (!SOUND_BUFFER.loadFromFile(audio_file)) {
		std::cout << "Failed to load audio file into buffer" << std::endl;
		return false;
	}
	return true;
}

void WindowRenderer::set_delay(const unsigned int& delay) noexcept
{
	delay_in_ms = delay;
	if (delay_in_ms == 0)
		window.setFramerateLimit(0);
	else
		window.setFramerateLimit(1000 / delay_in_ms);
}

std::vector<SortingElement>& WindowRenderer::get_array_ref()  noexcept
{
	return rectangle_array;
}

void WindowRenderer::initialize(const std::vector<int>& list) {
	// Generate the rectangles
	set_rectangle_data(list);
	create_rectangles(list);

	// Create the sound with the appropriate volume
	sf::Sound sound(SOUND_BUFFER);
	sound.setVolume(SOUND_VOLUME);
	sound_array = std::list<sf::Sound>(MAX_SOUNDS, sound);
}

void WindowRenderer::step() noexcept
{	

	// Event-Loop to keep things working
	sf::Event ev;
	while (window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window.close();
		}

		// Way to close the window
		if (ev.type == sf::Event::KeyPressed) {
			if (ev.key.code == sf::Keyboard::Escape) {
				window.close();
			}
		}
	}

	generate_and_draw_text();
	draw_rectangles();
	display();
	clear();
}

bool WindowRenderer::is_window_alive() const
{
	return window.isOpen();
}

void WindowRenderer::close() {
	return window.close();
}

bool WindowRenderer::poll_event(sf::Event& event)
{
	return window.pollEvent(event);
}

void WindowRenderer::add_sound(const int& curr_value)
{
	float half_max = rect_data.max_value / 2.0;
	float relative_value = (curr_value - half_max) / half_max;                 // High values - high pitch | Low values - low pitch
	float positive_value = abs(relative_value + 1) + MINIMUM_PITCH + 1e-5;	   // [-1;1] => (0; 2] + MINIMUM_PITCH 

	// If the array of sounds is full -> start over
	static int SOUND_COUNTER = 0;
	if (SOUND_COUNTER >= MAX_SOUNDS - 1) {
		SOUND_COUNTER = 0;
	}

	auto iter = sound_array.begin();
	std::advance(iter, SOUND_COUNTER);
	// In effect - the same as resetting the audio
	iter->stop();
	iter->setPitch(positive_value);
	iter->play();

	SOUND_COUNTER++;
}


// Currently only used to display millisecond delay. TODO: Add Read/Write counts
void WindowRenderer::generate_and_draw_text() noexcept
{
	// Rendering Delay in ms
	auto delay = sf::Text();
	auto font = sf::Font();
	font.loadFromFile("minecraft.ttf");
	delay.setFont(font);

	delay.setPosition(0, 0);
	delay.setString("Delay: " + std::to_string(delay_in_ms) + "ms");
	delay.setCharacterSize(20);
	delay.setFillColor(sf::Color::White);
	window.draw(delay);


	// Rendering All comparisons / reads


	// Rendering Algorithm name

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
	// Width of a rectangle
	rect_data.size.x = window.getSize().x / static_cast<float>(list.size());
	// How much (%) does a value of '1' take up
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
	
		rectangle_array.emplace_back(SortingElement( rect, elem ));
	}
}

void WindowRenderer::swap_rectangle_positions(const int idx1, const int idx2) noexcept
{
	if (idx1 == idx2)
		return;

	// Determine their x positions, and swap them accordingly
	const auto x1 = rectangle_array.at(idx1).get_pos().x;
	const auto x2 = rectangle_array.at(idx2).get_pos().x;

	// Move rectangles forward by the differences in x positions
	rectangle_array.at(idx1).move_by(sf::Vector2f(x2 - x1, 0));
	rectangle_array.at(idx2).move_by(sf::Vector2f(x1 - x2, 0));
}

void WindowRenderer::set_rectangle_color(const int& idx, const sf::Color color) noexcept
{
	rectangle_array.at(idx).set_color(color);
}