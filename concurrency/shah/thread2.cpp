#include <iostream>
#include <thread>

int main()
{
    auto lambda = [](int x) {
        std::cout << "Hello from thread!" << std::endl;
        std::cout << "Argument passed in: " << x << std::endl;
    };

    std::thread myThread(lambda, 100);
    myThread.join();

    std::cout << "Main is complete" << std::endl;
    return 0;
}
