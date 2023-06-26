// g++ async.cpp -std=c++20 -lpthread
#include <future>
#include <iostream>

int square(int x)
{
    return x * x;
}

int main()
{
    std::future<int> async_func = std::async(&square, 12);
    std::cout
        << "result: "
        << async_func.get()
        << std::endl;
    return 0;
}
