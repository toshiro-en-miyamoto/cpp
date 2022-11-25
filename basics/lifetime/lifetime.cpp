#include <string>
#include <iostream>
#include <utility>

static constexpr std::string_view tab{"    "}, comment{"// "};

void print(const std::string& name, char c1, const void* a1) {
    std::cout << tab << comment << name
    << " X{'" << c1 << "'} @ " << a1 << '\n';
}

void print(const std::string& name, char c1, const void* a1, const void* a2) {
    std::cout << tab << comment << name
    << " X{'" << c1 << "'} @ " << a1
    << " <="
    << " X{'" << c1 << "'} @ " << a2 << '\n';
}

class X
{
    char c;
public:
    X() : c{'~'} {
        print("new ", '~', this);
    }
    X(char arg) : c{arg} {
        print("new ", c, this);
    }
    ~X() {
        print("del ", c, this);
    }
    X(const X& x) : c{x.c} {
        print("ctor", c, this, &x);
    }
    X& operator=(const X& x) {
        c = x.c;
        print("copy", c, this, &x);
        return *this;
    }
    X(X&& x) : c{std::exchange(x.c, '~')} {
        print("mtor", c, this, &x);
    }
    X& operator=(X&& x) {
        c = std::exchange(x.c, '~');
        print("move", c, this, &x);
        return *this;
    }
};

X ident(X arg)
{
    return arg;
}

int main()
{
    std::cout
    << "int main()\n"
    << '{'
    << '\n';

    std::cout
    << tab
    << "X x1;"
    << '\n';
    X x1;

    std::cout << '\n'
    << tab
    << "X x2{'2'};"
    << '\n';
    X x2{'2'};

    std::cout << '\n'
    << tab
    << "x1 = ident(x2);"
    << '\n';
    x1 = ident(x2);

    std::cout << '\n'
    << tab
    << "x1 = x2;"
    << '\n';
    x1 = x2;

    std::cout
    << "}"
    << '\n';
}
