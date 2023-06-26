#include <iostream>
#include <thread>
#include <vector>

int main()
{
    auto lambda = [](int x) {
        std::cout
        << "thread: " << std::this_thread::get_id()
        << std::endl
        << "argument passed in: " << x
        << std::endl;
    };

    {
        constexpr const std::size_t instances = 100;
        std::vector<std::jthread> jthreads;

        for (int i = 0; i < instances; i++) {
            jthreads.push_back(std::jthread(lambda, i));
        }
    }

    std::cout << "Main is complete" << std::endl;
    return 0;
}
