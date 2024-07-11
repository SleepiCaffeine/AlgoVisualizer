#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <vector>
#include <list>

struct RectangleShapeData {
	sf::Vector2f size;
	int max_value;
};

struct SortingElement {
	sf::RectangleShape rect;
	int val;
};


class WindowRenderer
{

private:

	sf::SoundBuffer					SOUND_BUFFER;
	std::list <sf::Sound>			sound_array;

	std::vector<SortingElement> rectangle_array;
	sf::RenderWindow window;

	RectangleShapeData	 rect_data;
	unsigned int	     delay_in_ms;
	
	void set_rectangle_data(const std::vector<int>& list);
public:

	WindowRenderer(const sf::VideoMode video_mode, const unsigned int& delay_in_ms,
				   const sf::String& title = "algo", const sf::String& audio_file = "read_sound.ogg");

	// Handling window processes
	void display() noexcept;
	void clear() noexcept;
	void initialize(const std::vector<int>& list);
	void step() noexcept;
	bool is_window_alive() const;

	// Handling audio

	void remove_finished_sounds();
	void add_sound(const int& curr_value);

	// Handling text
	// Handling text
	void generate_and_draw_text() noexcept;

	// Handling Rectangles
	void create_rectangles(const std::vector<int>& list);
	void swap_rectangle_positions(const int idx1, const int idx2) noexcept;
	void draw_rectangles() noexcept;
	void set_rectangle_color(const int& idx, const sf::Color color) noexcept;
	void TEST_RECTANGLE_SWAPS();


	// Setters
	void set_title(const sf::String& title) noexcept;
	bool set_audio_file(const sf::String& audio_file) noexcept;
	void set_delay(const unsigned int& delay_in_ms) noexcept;

	// Getters
	std::vector<int> get_list_values() noexcept;

};

