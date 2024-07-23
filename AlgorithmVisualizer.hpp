#pragma once
#include "WindowRenderer.hpp"

#define READ_COLOR sf::Color::Green
#define WRITE_COLOR sf::Color::Yellow


struct Element {
	Ushort val;
	sf::RectangleShape rect;

	bool operator<=>(const Element&) const = default;
};


class AlgorithmVisualizer
{
private:
	WindowRenderer window_renderer;
	std::vector<Element> elements;

	using ULL = unsigned long long;
	void set_color(const ULL& idx, const sf::Color c);


public:
	explicit AlgorithmVisualizer(const std::vector<Ushort>& arr);

	// Implementing Primitive Vector functionality to have custom side-effects
	// Not implementing anything that modifies vector size, as that would require a lot of rectangle redrawing
	// Element Access
	
	const Ushort& get_at(const ULL index);
	void          set_at(const ULL index, const Ushort& value);
	const Ushort& front();
	const Ushort& back();
	std::vector<Ushort>     to_vector() const;


	// Modifiers
	void swap(const ULL idx1, const ULL idx2);


	// Capacity
	ULL  size()  const noexcept;
	bool empty() const noexcept;




	void test_read();
	void set_active(const bool active);
	void step();
	void clear(const sf::Color c);
	void close();
	void display();
	bool get_event(sf::Event& e);
	bool is_window_alive();
	void poll_event();
};

