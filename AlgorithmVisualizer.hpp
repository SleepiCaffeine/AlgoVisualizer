#pragma once
#include "WindowRenderer.hpp"

// Class to contain window and interface
class AlgorithmVisualizer
{
public:
	enum SortingMethod {
		NONE,
		BubbleSort,
		MergeSort,
		QuickSort,
	};



private:
	WindowRenderer window_renderer;
	SortingMethod  sorting_method;

public:



	AlgorithmVisualizer(const sf::VideoMode video_mode, const sf::String& audio_file_name);
	void start();

	void set_title(const std::string& title);
	void set_delay(const unsigned int delay);
	void initialize_list(const std::vector<int>& list);

	void set_sorting_method(SortingMethod method);
};

