// g++ async_buffer.cpp -std=c++20 -lpthread
#include <future>
#include <chrono>
#include <iostream>

size_t buffered()
{
    size_t bytes = 0;
    while (bytes < 20'000) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(260ms);
        bytes += 1000;
        std::cout << "loaded " << bytes << std::endl;
    }
    return bytes;
}

int main()
{
    std::future<size_t> background = std::async(
        std::launch::async,
        buffered
    );
    std::future_status status;

    // main loop
    bool quit = false;
    while (!quit) {
        // artificial sleep
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(50ms);
        // check the status
        status = background.wait_for(1ms);
        switch (status) {
            case std::future_status::timeout:
                std::cout << "not ready yet\n";
                break;
            case std::future_status::ready:
                std::cout << "future status ready!\n";
                quit = true;
                break;
            default:
                std::cout << "neither timeout nor ready\n";
        }
    }

    return 0;
}
