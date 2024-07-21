#include "AlgorithmVisualizer.hpp"
#include <thread>
#include <mutex>



using ULL = unsigned long long;

inline void AlgorithmVisualizer::set_color(const ULL& idx, const sf::Color c)
{
	window_renderer.set_color_at(idx, c);
}

AlgorithmVisualizer::AlgorithmVisualizer(const std::vector<Ushort>& arr) :
	window_renderer(WindowConfig(), arr)
{

	auto rectangles = window_renderer.get_rectangles();
	for (ULL i = 0; i < arr.size(); i++) {
		elements.push_back({ arr.at(i), rectangles.at(i) });
	}
}

const Ushort& AlgorithmVisualizer::get_at(const ULL index)
{
	set_color(index, sf::Color::Green);
	return elements.at(index).val;
}

void AlgorithmVisualizer::set_at(const ULL index, const Ushort& value)
{
	set_color(index, WRITE_COLOR);
	elements.at(index).val = value;
	window_renderer.set_value_at(value, index);
}

const Ushort& AlgorithmVisualizer::front()
{
	set_color(elements.size() - 1, READ_COLOR);
	return elements.front().val;
}

const Ushort& AlgorithmVisualizer::back()
{
	set_color(elements.size() - 1, READ_COLOR);
	return elements.back().val;
}

std::vector<Ushort> AlgorithmVisualizer::to_vector() const
{
	std::vector<Ushort> vec;

	for (const auto& e : elements)
		vec.push_back(e.val);

	return vec;
}

void AlgorithmVisualizer::swap(const ULL idx1, const ULL idx2)
{
	using std::swap;
	swap(elements.at(idx1), elements.at(idx2));

	window_renderer.swap(idx1, idx2);

	set_color(idx1, WRITE_COLOR);
	set_color(idx2, WRITE_COLOR);
}

constexpr ULL AlgorithmVisualizer::size() const noexcept 
{
	return elements.size();
}

constexpr bool AlgorithmVisualizer::empty() const noexcept
{
	return elements.empty();
}

void AlgorithmVisualizer::test_read()
{
	auto r = get_at(9);
}

void AlgorithmVisualizer::poll_event()
{
	window_renderer.poll_event();
}