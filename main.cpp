#include "VisualArray.hpp"

#include <numeric> // std::iota
#include <algorithm>
#include <random>
#include <ranges>
#include <thread>
#include <utility>
#include <mutex>

using ULL = unsigned long long;
bool can_draw = true;


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
}

unsigned int partition(VisualArray& va, const unsigned int low, const unsigned int high) {
    unsigned int pivot = va.get_at(low);
    va.step();


    int i = low - 1;
    int j = high + 1;

    while (true) {

        do {
            ++i;
            va.step();
        } while (va.get_at(i) < pivot);

        do {
            --j;
            va.step();
        } while (va.get_at(j) > pivot);




        if (i >= j) return j;

        using std::swap;
        va.swap(i, j);
        va.step();
    }

}

static void quickSort(VisualArray& va, const unsigned int low, const unsigned int high) {

    if (low >= high)
        return;

    unsigned int pivot = partition(va, low, high);
    quickSort(va, low, pivot);
    quickSort(va, pivot + 1, high);
}


std::mutex mutex;
VisualArray merge(VisualArray& l, VisualArray& r) noexcept {
    std::scoped_lock<std::mutex> lock(mutex);

    VisualArray result(l.get_wPtr(), 0);
    result.reserve(l.size() + r.size());

    while (!l.empty() && !r.empty()) {
        if (l.front() < r.front()) {
            result.push_back(l.front());
            result.get_wPtr().get()->set_active(true);
            result.step();
            result.get_wPtr().get()->set_active(false);
            l.erase(0);
        }

        else {
            result.push_back(r.front());
            result.get_wPtr().get()->set_active(true);
            result.step();
            result.get_wPtr().get()->set_active(false);
            r.erase(0);
        }
    }


    while (!l.empty()) {
        result.push_back(l.front());
        result.get_wPtr().get()->set_active(true);
        result.step();
        result.get_wPtr().get()->set_active(false);
        l.erase(0);
    }

    while (!r.empty()) {
        result.push_back(r.front());
        result.get_wPtr().get()->set_active(true);
        result.step();
        result.get_wPtr().get()->set_active(false);
        r.erase(0);
    }
   
    result.get_wPtr().get()->set_active(false);
    return result;
}

void mergeSort(VisualArray& va) noexcept {
    if (va.size() == 1)
        return;
    
    //std::scoped_lock<std::mutex> lock(mutex);
    int mid = va.size() / 2;
    // [0; mid)

    VisualArray left(va, 0, mid);
    left.get_wPtr().get()->set_active(false);
    // [mid; size)
    VisualArray right(va, mid, va.size());
    right.get_wPtr().get()->set_active(false);

    mergeSort(left);
    mergeSort(right);
    
    va = merge(left, right);
}


int main()
{
    // Create a randomized array
    std::vector<unsigned short> arr(1000);
    std::iota(arr.begin(), arr.end(), 1);
    std::shuffle(arr.begin(), arr.end(), std::mt19937{std::random_device{}()});
    

    // Set window configuration (optional) & Creating WindowRenderer
    WindowConfig wc;
    auto wr = std::make_shared<WindowRenderer>(wc, arr);

    // Creating the visual array, and copying over the randomized array
    VisualArray vis_array(wr, 0);
    vis_array.copy(arr);


    // Deactivating OpenGL resource, to pass off to thread
    wr.get()->set_active(false);
    std::jthread t(mergeSort, std::ref(vis_array));
    // Removing draw functionality
    can_draw = false;
    // Main thread loop, to keep window alive
    while (wr.get()->is_open()) {

        wr.get()->poll_event();
 
        if (can_draw)
            wr.get()->step();
    
    }


    t.join();
    
    return 0;
}