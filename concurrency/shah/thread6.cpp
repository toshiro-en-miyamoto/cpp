#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex g_lock;
static int shared_value = 0;

void increment_shared_value()
{
    std::lock_guard<std::mutex> lock(g_lock);
    shared_value = shared_value + 1;
}

int main()
{
    {
        constexpr const std::size_t instances = 10'000;
        std::vector<std::jthread> jthreads;

        for (int i = 0; i < instances; i++) {
            jthreads.push_back(std::jthread(increment_shared_value));
        }
    }

    std::cout << "result: " << shared_value << std::endl;
    return 0;
}
