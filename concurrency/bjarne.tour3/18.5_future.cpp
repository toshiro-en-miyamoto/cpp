// g++ 18.5_future.cpp -std=c++20 -lpthread -I/usr/local/include
#include <vector>
#include <numeric>

double accum(
    std::vector<double>::iterator beg,
    std::vector<double>::iterator end,
    double init
) {
    return std::accumulate(beg, end, init);
}

#include <future>
/**
 * It is rarely necessary to manually parallelize a standard-library
 * algorithm, such as accumulate(), because the parallel algorithms
 * (e.g., reduce(par_unseq, ...)) usually do a better job at that.
*/

double comp2(std::vector<double>& v)
{
    using it = std::vector<double>::iterator;
    std::packaged_task<double(it, it, double)> pt0 {accum};
    std::packaged_task<double(it, it, double)> pt1 {accum};

    std::future<double> f0 = {pt0.get_future()};
    std::future<double> f1 = {pt1.get_future()};

    auto mid = v.begin() + v.size()/2;
    std::jthread t0 {std::move(pt0), v.begin(), mid, 0.0};
    std::jthread t1 {std::move(pt1), mid, v.end(), 0.0};

    return f0.get() + f1.get();
}

/**
 * Please note that async() is not just a mechanism specialized for
 * parallel computation for increased performance. For example, it
 * can also be used to spawn a task for getting information from a
 * user, leaving the “main program” active with something else.
*/
double comp4(std::vector<double>& v)
{
    auto beg = v.begin();
    auto sz = v.size() / 4;
    auto p1 = beg + sz;
    auto p2 = beg + 2 * sz;
    auto p3 = beg + 3 * sz;

    auto f0 = std::async(accum, beg, p1, 0.0);
    auto f1 = std::async(accum, p1, p2, 0.0);
    auto f2 = std::async(accum, p2, p3, 0.0);
    auto f3 = std::async(accum, p3, v.end(), 0.0);

    return f0.get() + f1.get() + f2.get() + f3.get();
}

#include <stop_token>
#include <chrono>

using namespace std::chrono_literals;
constexpr auto INTERVAL = 200ms;
constexpr int CYCLE = 5;

int counting(std::stop_token token, int value)
{
    while (!token.stop_requested()) {
        value++;
        std::this_thread::sleep_for(INTERVAL);
    }
    return value;
}

int stop_counting()
{
    std::stop_source stop {};
    auto count = std::async(counting, stop.get_token(), 0);

    std::this_thread::sleep_for(INTERVAL * CYCLE);
    stop.request_stop();

    return count.get();
}

#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;

    "accum"_test = []
    {
        std::vector<double> v {1.0, 2.0, 3.0, 4.0};
        double sum = accum(v.begin(), v.end(), 0);
        expect(sum == 10.0_d);
    };

    "comp2"_test = []
    {
        std::vector<double> v {1.0, 2.0, 3.0, 4.0, 5.0};
        double sum = comp2(v);
        expect(sum == 15.0_d);
    };

    "comp4"_test = []
    {
        std::vector<double> v {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
        double sum = comp4(v);
        expect(sum == 21.0_d);
    };

    "stop_counting"_test = []
    {
        expect(that % stop_counting() == CYCLE);
    };

    return 0;
}
