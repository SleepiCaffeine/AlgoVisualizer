#include "AlgorithmVisualizer.hpp"

AlgorithmVisualizer::AlgorithmVisualizer(const sf::VideoMode video_mode, const sf::String& audio_file_name)
	: window_renderer(video_mode, 16, "Algoithm Visualizer", audio_file_name), sorting_method(NONE)
{ }

unsigned int partition(WindowRenderer& wr, std::vector<SortingElement>& vec, const unsigned int low, const unsigned int high) {
	unsigned int pivot = vec.at(low).get_val();

	int i = low - 1;
	int j = high + 1;

	while (true) {

		do {
			++i;
			vec.at(i).set_color(sf::Color::Green);
			wr.add_sound(vec.at(i).get_val());
		} while (vec.at(i).get_val() < pivot);

		do {
			--j;
			vec.at(j).set_color(sf::Color::Green);
			wr.add_sound(vec.at(j).get_val());
		} while (vec.at(j).get_val() > pivot);




		if (i >= j) return j;


		vec.at(i).set_color(sf::Color::Green);
		vec.at(j).set_color(sf::Color::Green);

		using std::swap;
		swap(vec.at(i), vec.at(j));
		wr.add_sound(vec.at(i).get_val());
		wr.add_sound(vec.at(j).get_val());
		wr.swap_rectangle_positions(i, j);
		wr.step();
	}

}

void quicksort(WindowRenderer& wr, std::vector<SortingElement>& vec, const unsigned int low, const unsigned int high) {
	if (low >= high)
		return;

	unsigned int pivot = partition(wr, vec, low, high);
	vec.at(pivot).set_color(sf::Color::Yellow);
	wr.step();

	quicksort(wr, vec, low, pivot);
	quicksort(wr, vec, pivot + 1, high);
}


void AlgorithmVisualizer::start()
{

	auto& list = window_renderer.get_array_ref();
	

	switch (sorting_method) {
	case SortingMethod::NONE:
		std::abort();
		
	case SortingMethod::BubbleSort:
		// TODO: Remove the sorting array from the window renderer, and facilitate communication through different channels
	for (int i = 0; i < list.size() -  1; i++) {

		bool is_sorted = true;

		for (int j = 0; j < list.size() - 1 - i; j++) {

			// Mark both elements as green (read), and update the visuals
			list.at(j).set_color(sf::Color::Green);
			list.at(j + 1).set_color(sf::Color::Green);
			window_renderer.step();

			// if left value > right value
			if (list.at(j) > list.at(j + 1)) {

				window_renderer.swap_rectangle_positions(j, j + 1);
				window_renderer.step();

				using std::swap;
				swap(list.at(j), list.at(j + 1));

				window_renderer.add_sound(list.at(j + 1).get_val());
				window_renderer.add_sound(list.at(j).get_val());

				is_sorted = false;
			}
		}

		if (is_sorted) return;
	}
	break;

	case SortingMethod::MergeSort:
		std::abort();

	case SortingMethod::QuickSort:
		quicksort(window_renderer, list, 0, static_cast<unsigned int>(list.size()) - 1);
		break;
	}

	while (window_renderer.is_window_alive()) {
		sf::Event ev;
		while (window_renderer.poll_event(ev)) {

			if (ev.type == sf::Event::EventType::Closed) {
				window_renderer.close();
			}
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
}

void AlgorithmVisualizer::set_sorting_method(SortingMethod method)
{
	sorting_method = method;
}
