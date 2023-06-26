// g++ cond_var.cpp -std=c++20 -lpthread
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <iostream>

std::atomic<int> result;

std::mutex g_lock;
std::condition_variable g_cond_var;
bool notified = false;

int main()
{
    std::jthread reporter([&]{
        std::unique_lock<std::mutex> lock(g_lock);

        //  while (!notified) {
        //      g_cond_var.wait(lock)
        //  }
        g_cond_var.wait(lock, []{return notified;});

        std::cout << "result: " << result << std::endl;
    });

    std::jthread worker([&]{
        std::unique_lock<std::mutex> lock(g_lock);

        result = 42 + 1 + 7;

        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Work complete" << std::endl;

        notified = true;
        g_cond_var.notify_one();
    });

    return 0;
}
