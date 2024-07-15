#pragma once
#include <SFML/Graphics.hpp>

class SortingElement
{
private:
	sf::RectangleShape rectangle;
	unsigned int value;

public:
	SortingElement();
	SortingElement(const sf::RectangleShape& rect, const unsigned int val);

	unsigned int get_val() const noexcept;
	sf::Vector2f get_pos() const noexcept;
	// get gud scrub
	sf::RectangleShape get_rect() const noexcept;

	void set_pos(const sf::Vector2f& pos)         noexcept;
	void move_by(const sf::Vector2f& offset)      noexcept;
	void set_color(const sf::Color& color)        noexcept;
	void set_size(const sf::Vector2f& size)       noexcept;
	void set_rect(const sf::RectangleShape& rect) noexcept;
	constexpr void set_val(const unsigned int val)          noexcept;


	constexpr bool operator< (const SortingElement& other) const noexcept;
	bool operator> (const SortingElement& other) const noexcept;
	bool operator<= (const SortingElement& other) const noexcept;
	constexpr bool operator>= (const SortingElement& other) const noexcept;
	constexpr bool operator==(const SortingElement& other) const noexcept;
	constexpr bool operator!= (const SortingElement& other) const noexcept;

	friend void swap(SortingElement& a, SortingElement& b) noexcept;
};

