#pragma once
#include <memory>
#include "WindowRenderer.hpp"


class VisualArray
{



	// TODO: Add deez nuts as an ending screen

private:
	std::shared_ptr<WindowRenderer> window_ptr;
	size_t deleted_elements = 0;
	size_t offset = 0;
	bool change_colors = true;
	std::vector<size_t> elements;


	void within_bounds(const size_t idx) const;
	void change_color(const size_t idx, const sf::Color& c) const noexcept;
	void play_sound(const size_t& value) const noexcept;

public:
	VisualArray(const std::shared_ptr<WindowRenderer>& wPtr, const size_t offset) noexcept;
	VisualArray(const VisualArray& other) noexcept;
	VisualArray(const VisualArray& other, const size_t begin, const size_t end) noexcept;

	~VisualArray() = default;
	VisualArray& operator=(const VisualArray& other) = default;


	[[nodiscard]] const std::shared_ptr<WindowRenderer>& get_wPtr() const noexcept;
	[[nodiscard]] size_t get_at(const size_t idx) const;
    void set_at(const size_t idx, const size_t& value);

	[[nodiscard]] size_t front() const noexcept;
	[[nodiscard]] size_t back()  const noexcept;

	[[nodiscard]] size_t size() const noexcept;
	[[nodiscard]] bool empty() const noexcept;

	void push_back(const size_t& e) noexcept;
	void erase(const size_t idx);

	void swap(const size_t idx1, const size_t idx2);
	void copy_from(const VisualArray& other, const size_t begin, const size_t end);
	void copy_from(const std::vector<size_t>& vect, const size_t begin, const size_t end);
	void copy(const std::vector<size_t>& vect) noexcept;

	void set_draw(const bool active = true) const noexcept;

	void reserve(const size_t space);

	void set_change_colors(const bool active = true) noexcept;

	void step() const noexcept;


	void read_and_play_sound() const;
};

