#include "VisualArray.hpp"
#include <stdexcept>

#define READ_COLOR  sf::Color::Green
#define WRITE_COLOR sf::Color::Yellow

using uint = unsigned int;

template <typename T>
[[nodiscard]] T to(const auto& value) noexcept { return static_cast<T>(value); }

void VisualArray::swap(const uint idx1, const uint idx2)
{
	within_bounds(idx1);
	within_bounds(idx2);

	std::swap(elements.at(idx1), elements.at(idx2));

	window_ptr.get()->swap(idx1 + offset, idx2 + offset);
	change_color(idx1, READ_COLOR);
	change_color(idx2, READ_COLOR);
}

// Copies values [begin; end)
void VisualArray::copy_from(const VisualArray& other, const uint begin, const uint end)
{
	offset += begin;
	copy_from(other.elements, begin, end);
}

void VisualArray::copy_from(const std::vector<sf::Uint16>& vect, const uint begin, const uint end)
{
	if (vect.size() <= begin || vect.size() < end)
		throw std::runtime_error("Accessing array out of bounds");

	if (end <= begin)
		throw std::runtime_error("Improper indexing when specifying vector slice");

	auto new_begin = std::next(vect.begin(), begin);
	auto new_end   = std::next(vect.begin(), end);

	elements = std::vector<sf::Uint16>(new_begin, new_end);

}

void VisualArray::copy(const std::vector<sf::Uint16>& vect) noexcept {
	elements = vect;
}
void VisualArray::reserve(const uint space)
{
	elements.reserve(space);
}
void VisualArray::set_change_colors(const bool active) noexcept
{
	change_colors = active;
}

void VisualArray::step() const noexcept
{
	window_ptr.get()->step();
}


void VisualArray::within_bounds(const uint idx) const
{
	if (idx >= elements.size())
		throw std::out_of_range("Accessing array out of bounds");
}

void VisualArray::change_color(const uint idx, const sf::Color& c) const noexcept
{
	if (change_colors)
		window_ptr.get()->set_color_at(idx + offset, c);
}

VisualArray::VisualArray(const std::shared_ptr<WindowRenderer>& wPtr, const uint off) noexcept
	: window_ptr{ wPtr }, offset(off)
{ }

VisualArray::VisualArray(const VisualArray& other) noexcept : VisualArray(other, 0, to<uint>(other.size()))
{ }

VisualArray::VisualArray(const VisualArray& other, const uint begin, const uint end) noexcept
	: window_ptr{ other.window_ptr }, offset(other.offset) {
	copy_from(other, begin, end);
}

const std::shared_ptr<WindowRenderer>& VisualArray::get_wPtr() const noexcept
{
	return window_ptr;
}

sf::Uint16 VisualArray::get_at(const uint idx) const
{
	within_bounds(idx);
	change_color(idx, READ_COLOR);
	return elements.at(idx);
}

void VisualArray::set_at(const uint idx, const sf::Uint16& value)
{
	within_bounds(idx);
	change_color(idx, WRITE_COLOR);
	elements.at(idx) = value;
}

sf::Uint16 VisualArray::front() const noexcept
{
	change_color(0, READ_COLOR);
	return elements.front();
}

sf::Uint16 VisualArray::back() const noexcept
{
	change_color(to<uint>(elements.size() - 1), READ_COLOR);
	return elements.back();
}

size_t VisualArray::size() const noexcept
{
	return elements.size();
}

bool VisualArray::empty() const noexcept
{
	return elements.empty();
}

void VisualArray::push_back(const sf::Uint16& e) noexcept
{
	elements.push_back(e);
	window_ptr->set_value_at(elements.back(), offset + (elements.size() - 1));
	change_color(elements.size() - 1, WRITE_COLOR);
}

void VisualArray::erase(const uint idx)
{
	within_bounds(idx);
	elements.erase(std::next(elements.begin(), idx));
}
