// g++ try_lock.cpp -std=c++20 -lpthread
// $ ./a.out
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>

std::mutex g_lock;
bool quit = false;

void background()
{
    constexpr const unsigned int MAX_LOOP = 50;
    quit = false;
    for (unsigned int i = 0; i < MAX_LOOP; i++) {
        using namespace std::chrono_literals;
        {
            std::lock_guard lock(g_lock);
            // do some work
            std::this_thread::sleep_for(19ms);
        }
        std::this_thread::sleep_for(61ms);
    }
    quit = true;
}

int main()
{
    std::thread thread1(background);

    // main loop
    while (!quit) {
        static int counter = 0;
        counter++;
 
        using namespace std::chrono_literals;
        if (g_lock.try_lock()) {
            std::lock_guard lock(g_lock, std::adopt_lock);
            // do something with background
            std::cout << counter << ": acquired\n";
            std::this_thread::sleep_for(3ms);
        } else {
            std::cout << counter << ": missed the lock\n";
        }
        // 30 FPS
        std::this_thread::sleep_for(30ms);
    }

    thread1.join();
    return 0;
}