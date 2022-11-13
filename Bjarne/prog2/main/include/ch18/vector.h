#pragma one

#include <initializer_list>

namespace ch18 {

class vector
{
    std::size_t sz;
    double* elem;
public:
    vector();
    explicit vector(std::size_t);
    vector(std::initializer_list<double>);
    vector(const vector&);
    vector& operator=(const vector&);
    vector(vector&&);
    vector& operator=(vector&&);
    ~vector();

    double& operator[](std::size_t);
    double operator[](std::size_t) const;
};

}
