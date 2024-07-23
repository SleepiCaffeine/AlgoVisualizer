#include "VisualArray.hpp"
#include <stdexcept>

#define READ_COLOR  sf::Color::Green
#define WRITE_COLOR sf::Color::Yellow

using uint = unsigned int;

void VisualArray::swap(const uint idx1, const uint idx2) noexcept
{
	within_bounds(idx1);
	within_bounds(idx2);

	std::swap(elements.at(idx1), elements.at(idx2));

	window_ptr.get()->swap(idx1 + offset, idx2 + offset);
	window_ptr.get()->set_color_at(idx1 + offset, READ_COLOR);
	window_ptr.get()->set_color_at(idx2 + offset, READ_COLOR);
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
	auto new_end = std::next(vect.begin(), end);

	elements = std::vector<sf::Uint16>(new_begin, new_end);

}

void VisualArray::copy(const std::vector<sf::Uint16>& vect) {
	elements = vect;
}


void VisualArray::within_bounds(const uint idx) const
{
	if (idx >= elements.size())
		throw std::runtime_error("Accessing array out of bounds");
}

VisualArray::VisualArray(const std::shared_ptr<WindowRenderer>& wPtr, const uint off)
	: window_ptr{ wPtr }, offset(off)
{ }

VisualArray::VisualArray(const VisualArray& other) : VisualArray(other, 0, other.size())
{ }

VisualArray::VisualArray(const VisualArray& other, const uint begin, const uint end)
	: window_ptr{ other.window_ptr }, offset(other.offset) {
	copy_from(other, begin, end);
}

const std::shared_ptr<WindowRenderer>& VisualArray::get_wPtr() const
{
	return window_ptr;
}

sf::Uint16 VisualArray::get_at(const uint idx) const
{
	within_bounds(idx);

	window_ptr.get()->set_color_at(idx + offset, READ_COLOR);
	return elements.at(idx);
}

void VisualArray::set_at(const uint idx, const sf::Uint16& value)
{
	within_bounds(idx);
	window_ptr.get()->set_color_at(idx + offset, WRITE_COLOR);
	elements.at(idx) = value;
}

sf::Uint16 VisualArray::front() const noexcept
{
	window_ptr.get()->set_color_at(0 + offset, READ_COLOR);
	return elements.front();
}

sf::Uint16 VisualArray::back() const noexcept
{
	window_ptr.get()->set_color_at(elements.size() - 1 + offset, READ_COLOR);
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
