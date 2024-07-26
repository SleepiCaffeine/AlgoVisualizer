#pragma once
#include <memory>
#include "WindowRenderer.hpp"


class VisualArray
{

private:
	std::shared_ptr<WindowRenderer> window_ptr;
	size_t deleted_elements = 0;
	size_t offset = 0;
	bool change_colors = true;
	std::vector<sf::Uint16> elements;


	void within_bounds(const size_t idx) const;
	void change_color(const size_t idx, const sf::Color& c) const noexcept;

public:
	VisualArray(const std::shared_ptr<WindowRenderer>& wPtr, const size_t offset) noexcept;
	VisualArray(const VisualArray& other) noexcept;
	VisualArray(const VisualArray& other, const size_t begin, const size_t end) noexcept;

	~VisualArray() = default;
	VisualArray& operator=(const VisualArray& other) = default;


	[[nodiscard]] const std::shared_ptr<WindowRenderer>& get_wPtr() const noexcept;
	[[nodiscard]] sf::Uint16 get_at(const size_t idx) const;
    void set_at(const size_t idx, const sf::Uint16& value);

	[[nodiscard]] sf::Uint16 front() const noexcept;
	[[nodiscard]] sf::Uint16 back()  const noexcept;

	[[nodiscard]] size_t size() const noexcept;
	[[nodiscard]] bool empty() const noexcept;

	void push_back(const sf::Uint16& e) noexcept;
	void erase(const size_t idx);

	void swap(const size_t idx1, const size_t idx2);
	void copy_from(const VisualArray& other, const size_t begin, const size_t end);
	void copy_from(const std::vector<sf::Uint16>& vect, const size_t begin, const size_t end);
	void copy(const std::vector<sf::Uint16>& vect) noexcept;

	void set_draw(const bool active = true) const noexcept;

	void reserve(const size_t space);

	void set_change_colors(const bool active = true) noexcept;

	void step() const noexcept;
};

