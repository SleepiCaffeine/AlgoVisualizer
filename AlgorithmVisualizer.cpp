#include "AlgorithmVisualizer.hpp"

AlgorithmVisualizer::AlgorithmVisualizer(const sf::VideoMode video_mode, const sf::String& audio_file_name)
	: window_renderer(video_mode, 16, "Algoithm Visualizer", audio_file_name), sorting_method(NONE)
{ }

void AlgorithmVisualizer::start()
{
	while (window_renderer.is_window_alive())
	{

		switch (sorting_method) {
		case SortingMethod::NONE:
			std::abort();

		case SortingMethod::BubbleSort:

		for (int i = 0; i < list.size() - 1; i++) {
			for (int j = 0; j < list.size() - 1 - i; j++) {

				window_renderer.set_rectangle_color(j, sf::Color::Green);
				window_renderer.set_rectangle_color(j + 1, sf::Color::Green);
				window_renderer.step();

				// if left value > right value
				if (list.at(j) > list.at(j + 1)) {

					window_renderer.swap_rectangle_positions(j, j + 1);
					window_renderer.step();
					std::swap(list.at(j), list.at(j + 1));
					window_renderer.add_sound(list.at(j + 1));
					window_renderer.add_sound(list.at(j));
				}
			}
		}
		break;

		case SortingMethod::MergeSort:
			std::abort();

		case SortingMethod::QuickSort:
			std::abort();
		}
	}
}

void AlgorithmVisualizer::set_title(const std::string& title)
{
	window_renderer.set_title(title);
}

void AlgorithmVisualizer::set_delay(const unsigned int delay)
{
	window_renderer.set_delay(delay);
}

void AlgorithmVisualizer::initialize_list(const std::vector<int>& l)
{
	window_renderer.initialize(l);
	list = l;
}

void AlgorithmVisualizer::set_sorting_method(SortingMethod method)
{
	sorting_method = method;
}

