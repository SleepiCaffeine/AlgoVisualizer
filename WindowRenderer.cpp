#include "WindowRenderer.hpp"
#include <iostream>
#define MAX_SOUNDS 250


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
	for (auto& rect : rectangle_array) {
		window.draw(rect);
		rect.setFillColor(sf::Color::White);	// Reset Color
	}
}

void WindowRenderer::TEST_RECTANGLE_SWAPS()
{
	// Generate 2 random indecies to swap between
	// No need for fancy-shmancy std::uniform_int_distribution or std::mt19937
	int idx1 = rand() % (rectangle_array.size());
	int idx2 = rand() % (rectangle_array.size());

	swap_rectangle_positions(idx1, idx2);
}


void WindowRenderer::initialize(const std::vector<int>& list) {
	// Generate the rectangles
	set_rectangle_data(list);
	create_rectangles(list);
}

void WindowRenderer::step() noexcept
{	

	// Event-Loop to keep things working
	sf::Event ev;
	while (window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window.close();
		}
	}

	generate_and_draw_text();
	draw_rectangles();
	TEST_RECTANGLE_SWAPS();
	display();
	clear();
	//remove_finished_sounds();

}

bool WindowRenderer::is_window_alive() const
{
	return window.isOpen();
}

void WindowRenderer::add_sound(const int& curr_value)
{
	// Create the sound with the appropriate volume and pitch
	sf::Sound new_sound(SOUND_BUFFER);
	new_sound.setVolume(30);


	float half_max = rect_data.max_value / 2.0;
	float relative_value = (curr_value - half_max) / half_max;         // High values - high pitch | Low values - low pitch
	float positive_value = abs(relative_value + 1) + 1e-4;			   // [-1;1] => (0; 2]  
	new_sound.setPitch(positive_value);


	// if the array of sounds is full -> start over
	if (sound_array.size() >= MAX_SOUNDS - 1) {
		std::cout << "Clear\n";
		sound_array.clear();
	}

	// Add the sound and play it
	sound_array.emplace_back(new_sound);
	sound_array.back().play();
}

void WindowRenderer::remove_finished_sounds() {
	while (!sound_array.empty()											           	   // The array is not empty
			&&																		   // And
		  (sound_array.back().getPlayingOffset().asMilliseconds() >= delay_in_ms)) {   // Front Sound is taking longer than the delay
		sound_array.pop_back();									                       // Remove front sound
	}
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
	rect_data.size.x = static_cast<float>(window.getSize().x / list.size());
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
	
		rectangle_array.push_back(rect);
	}
}

void WindowRenderer::swap_rectangle_positions(const int& idx1, const int& idx2) noexcept
{
	if (idx1 == idx2)
		return;

	// Determine their x positions, and swap them accordingly
	const float x_1 = rectangle_array.at(idx1).getPosition().x;
	const float x_2 = rectangle_array.at(idx2).getPosition().x;
	// Move rectangles forward by the differences in x positions
	rectangle_array.at(idx1).move((x_2 - x_1), 0);
	rectangle_array.at(idx2).move((x_1 - x_2), 0);

	set_rectangle_color(idx1, sf::Color::Green);
	add_sound(idx1);

	set_rectangle_color(idx2, sf::Color::Green);
	add_sound(idx2);
}

void WindowRenderer::set_rectangle_color(const int& idx, const sf::Color color) noexcept
{
	rectangle_array.at(idx).setFillColor(color);
}

void WindowRenderer::clear_all_rectangle_colors() noexcept
{
	for (auto& r : rectangle_array)
		r.setFillColor(sf::Color::White);
}
