#pragma once
#include <memory>
#include "WindowRenderer.hpp"


class VisualArray
{
	using uint = unsigned int;
private:
	std::shared_ptr<WindowRenderer> window_ptr;
	unsigned int offset;
	std::vector<sf::Uint16> elements;


	void within_bounds(const uint idx) const;
public:
	VisualArray(const std::shared_ptr<WindowRenderer>& wPtr, const uint offset);
	VisualArray(const VisualArray& other);
	VisualArray(const VisualArray& other, const uint begin, const uint end);


	const std::shared_ptr<WindowRenderer>& get_wPtr() const;
	[[nodiscard]] sf::Uint16 get_at(const uint idx) const;
    void set_at(const uint idx, const sf::Uint16& value);

	sf::Uint16 front() const noexcept;
	sf::Uint16 back()  const noexcept;

	size_t size() const noexcept;
	bool empty() const noexcept;

	void swap(const uint idx1, const uint idx2) noexcept;
	void copy_from(const VisualArray& other, const uint begin, const uint end);
	void copy_from(const std::vector<sf::Uint16>& vect, const uint begin, const uint end);
	void copy(const std::vector<sf::Uint16>& vect);
};

