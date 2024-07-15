#include "SortingElement.hpp"

constexpr void SortingElement::set_val(unsigned int val) noexcept
{
	value = val;
}

SortingElement::SortingElement() : rectangle{sf::RectangleShape()}, value{ 0 }
{ }

SortingElement::SortingElement(const sf::RectangleShape & rect, const unsigned int val)
	: rectangle{ rect }, value{ val }
{ }

unsigned int SortingElement::get_val() const noexcept
{
	return value;
}

void SortingElement::set_rect(const sf::RectangleShape& rect) noexcept
{
	rectangle = rect;
}

sf::RectangleShape SortingElement::get_rect() const noexcept
{
	return rectangle;
}

sf::Vector2f SortingElement::get_pos() const noexcept
{
	return rectangle.getPosition();
}

void SortingElement::set_pos(const sf::Vector2f& pos) noexcept
{
	rectangle.setPosition(pos);
}

void SortingElement::move_by(const sf::Vector2f& offset) noexcept
{
	rectangle.move(offset);
}

void SortingElement::set_color(const sf::Color& color) noexcept
{
	rectangle.setFillColor(color);
}

void SortingElement::set_size(const sf::Vector2f& size) noexcept
{
	rectangle.setSize(size);
}

constexpr bool SortingElement::operator<(const SortingElement& other) const noexcept
{
	return this->value < other.value;
}

bool SortingElement::operator>(const SortingElement& other) const noexcept
{
	return this->value > other.value;
}

bool SortingElement::operator<=(const SortingElement& other) const noexcept
{
	return !(*this > other);
}

constexpr bool SortingElement::operator>=(const SortingElement& other) const noexcept
{
	return !(*this < other);
}

constexpr bool SortingElement::operator==(const SortingElement& other) const noexcept
{
	return this->value == other.value;
}

constexpr bool SortingElement::operator!=(const SortingElement& other) const noexcept
{
	return (*this == other);
}

void swap(SortingElement& a, SortingElement& b) noexcept
{
	using std::swap;
	swap(a.value, b.value);
	swap(a.rectangle, b.rectangle);
}
