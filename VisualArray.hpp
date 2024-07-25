#pragma once
#include <memory>
#include "WindowRenderer.hpp"


class VisualArray
{
public:
	using uint = unsigned int;

private:
	std::shared_ptr<WindowRenderer> window_ptr;
	unsigned int offset;
	bool change_colors = true;
	std::vector<sf::Uint16> elements;


	void within_bounds(const uint idx) const;
	void change_color(const uint idx, const sf::Color& c) const noexcept;

public:
	VisualArray(const std::shared_ptr<WindowRenderer>& wPtr, const uint offset) noexcept;
	VisualArray(const VisualArray& other) noexcept;
	VisualArray(const VisualArray& other, const uint begin, const uint end) noexcept;

	~VisualArray() = default;
	VisualArray& operator=(const VisualArray& other) = default;


	[[nodiscard]] const std::shared_ptr<WindowRenderer>& get_wPtr() const noexcept;
	[[nodiscard]] sf::Uint16 get_at(const uint idx) const;
    void set_at(const uint idx, const sf::Uint16& value);

	[[nodiscard]] sf::Uint16 front() const noexcept;
	[[nodiscard]] sf::Uint16 back()  const noexcept;

	[[nodiscard]] size_t size() const noexcept;
	[[nodiscard]] bool empty() const noexcept;

	void push_back(const sf::Uint16& e) noexcept;
	void erase(const uint idx);

	void swap(const uint idx1, const uint idx2);
	void copy_from(const VisualArray& other, const uint begin, const uint end);
	void copy_from(const std::vector<sf::Uint16>& vect, const uint begin, const uint end);
	void copy(const std::vector<sf::Uint16>& vect) noexcept;

	void reserve(const uint space);

	void set_change_colors(const bool active = true) noexcept;

	void step() const noexcept;
};

