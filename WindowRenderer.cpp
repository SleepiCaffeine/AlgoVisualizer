#include "WindowRenderer.hpp"
#include <iostream>
#define MAX_SOUNDS 50


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
		window.draw(r.rect);
		r.rect.setFillColor(sf::Color::White);	// Reset Color
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
	window.setFramerateLimit(1000 / delay_in_ms);
}

std::vector<int> WindowRenderer::get_list_values() noexcept
{
	std::vector<int> values;
	for (auto& r : rectangle_array) {
		values.push_back(r.val);
	}
	return values;
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

void WindowRenderer::add_sound(const int& curr_value)
{
	// Create the sound with the appropriate volume and pitch
	sf::Sound new_sound(SOUND_BUFFER);
	new_sound.setVolume(30);


	float half_max = rect_data.max_value / 2.0;
	float relative_value = (curr_value - half_max) / half_max;         // High values - high pitch | Low values - low pitch
	float positive_value = abs(relative_value + 1) + 1e-5;			   // [-1;1] => (0; 2]  
	new_sound.setPitch(positive_value);


	// If the array of sounds is full -> start over
	static int SOUND_COUNTER = 0;
	if (SOUND_COUNTER >= MAX_SOUNDS - 1) {
		SOUND_COUNTER = 0;
	}

	// Add the sound, by replacing an earlier sf::Sound object
	// Because sf::Sound.getPlayingOffset().asMicroseconds() doesn't work properly
	auto iter = sound_array.begin();
	std::advance(iter, SOUND_COUNTER);
	*iter = new_sound;
	iter->play();

	SOUND_COUNTER++;
}

void WindowRenderer::remove_finished_sounds() {
	while (true) {

		if (sound_array.empty()) {
			std::cout << "Array is empty" << std::endl;
			return;
		}
		if (sound_array.front().getPlayingOffset().asMicroseconds() < static_cast<sf::Int32>(delay_in_ms)) {
			std::cout << "Front Sound is taking less than the delay (" << sound_array.front().getPlayingOffset().asMicroseconds() << ")\n";
			return;
		}
			
		sound_array.erase(sound_array.begin());
		std::cout << "After Deletion, size is: " << sound_array.size() << std::endl;
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
	
		rectangle_array.push_back({ rect, elem });
	}
}

void WindowRenderer::swap_rectangle_positions(const int idx1, const int idx2) noexcept
{
	if (idx1 == idx2)
		return;

	// Determine their x positions, and swap them accordingly
	const auto pos_1 = rectangle_array.at(idx1).rect.getPosition();
	const auto pos_2 = rectangle_array.at(idx2).rect.getPosition();
	// Move rectangles forward by the differences in x positions
	rectangle_array.at(idx1).rect.setPosition(pos_2.x, pos_1.y);
	rectangle_array.at(idx2).rect.setPosition(pos_1.x, pos_2.y);

	std::swap(rectangle_array.at(idx1), rectangle_array.at(idx2));
}

void WindowRenderer::set_rectangle_color(const int& idx, const sf::Color color) noexcept
{
	rectangle_array.at(idx).rect.setFillColor(color);
}