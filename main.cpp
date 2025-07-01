#include "VisualArray.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>
#include <thread>
#include <utility>
#include <atomic>
#include <mutex>

std::atomic<bool> can_draw = true;
std::atomic<bool> ended = false;
std::mutex mutex;



void insertionSort(VisualArray& va) noexcept {

    can_draw = false;
    va.set_draw(true);

    for (size_t i = 0; i < va.size(); ++i) {
        
        size_t j = i;
        size_t x = va.get_at(i);

        while (j > 0 && x < va.get_at(j - 1)) {
            va.set_at(j, va.get_at(j - 1));
            va.step();
            --j;

        }
    
        va.set_at(j, x);
        va.step();
    }

    va.set_draw(false);
    can_draw = true;
    ended = true;

}



void bubblesort(VisualArray& va) {
    va.get_wPtr().get()->set_active(true);

    for (int i = 0; i < va.size() - 1; ++i) {
        for (int j = 0; j < va.size() - 1 - i; ++j) {
            if (va.get_at(j) > va.get_at(j + 1)) {
                va.swap(j, j + 1);
            }

            va.step();
        }
    }

    can_draw = true;
    ended = true;
}

size_t partition(VisualArray& va, const size_t& low, const size_t& high) {
    size_t pivot = va.get_at(low);
    va.set_draw(true);


    size_t i = low - 1;
    size_t j = high + 1;

    while (true) {

        do {
            ++i;
        } while (va.get_at(i) < pivot);
        va.step();

        do {
            --j;
        } while (va.get_at(j) > pivot);
        va.step();



        if (i >= j) return j;

        using std::swap;
        va.swap(i, j);
        va.step();
    }

    va.set_draw(false);

}

static void rec_quickSort(VisualArray& va, const size_t& low, const size_t& high) {

    if (low >= high)
        return;

    size_t pivot = partition(va, low, high);
    rec_quickSort(va, low, pivot);
    rec_quickSort(va, pivot + 1, high);

    va.set_draw(false);
}

static void quickSort(VisualArray& va, const size_t low, const size_t high) {
    std::unique_lock<std::mutex> lc(mutex);
    can_draw = false;
    rec_quickSort(va, low, high);
    va.set_draw(false);
    can_draw = true;
    ended = true;
}

VisualArray merge(VisualArray& l, VisualArray& r, const unsigned int offset) noexcept {

    VisualArray result(l.get_wPtr(), offset);
    result.reserve(l.size() + r.size());
    result.set_draw(true);

    while (!l.empty() && !r.empty()) {
        if (l.front() < r.front()) {
            result.push_back(l.front());
            l.erase(0);
        }

        else {
            result.push_back(r.front());
            r.erase(0);
        }

        result.step();
    }


    while (!l.empty()) {
        result.push_back(l.front());
        result.step();
        l.erase(0);
    }

    while (!r.empty()) {
        result.push_back(r.front());
        result.step();
        r.erase(0);
    }
   
    result.set_draw(false);
    return result;
}

void rec_mergeSort(VisualArray& va, const unsigned int offset) noexcept {
    if (va.size() == 1)
        return;
    
    size_t mid = va.size() / 2;
    // [0; mid)
    VisualArray left(va, 0, mid);
    // [mid; size)
    VisualArray right(va, mid, va.size());

    rec_mergeSort(left, offset);
    rec_mergeSort(right, offset + mid);
    
    va = merge(left, right, offset);
}

void mergeSort(VisualArray& va, const unsigned int offset = 0) noexcept {
    can_draw = false;
    std::unique_lock<std::mutex> lc(mutex);
    rec_mergeSort(va, offset);
    can_draw = true;
    ended = true;
}


int main()
{


    // Create a randomized array
    std::vector<size_t> arr(10000);
    std::iota(arr.begin(), arr.end(), 1);
    std::shuffle(arr.begin(), arr.end(), std::mt19937{std::random_device{}()});
    

    // Set window configuration (optional) & Creating WindowRenderer
    WindowConfig wc;
    wc.setOutline(false);
    wc.millisecond_delay = 50;
    auto wr = std::make_shared<WindowRenderer>(wc, arr);

    // Creating the visual array, and copying over the randomized array
    VisualArray vis_array(wr, 0);
    vis_array.copy(arr);

    // Deactivating OpenGL resource, to pass off to thread
    wr.get()->set_active(false);
    std::jthread t(quickSort, std::ref(vis_array), 0, vis_array.size() - 1);


    // Main thread loop, to keep window alive
    while (wr.get()->is_open()) {

        wr.get()->poll_event();

        if (ended) {
            vis_array.read_and_play_sound();
            ended = false;
        }

        if (can_draw) {
            wr.get()->set_active(true);
            wr.get()->step();
        }
        
 
    }
    
    t.join();

    return 0;
}