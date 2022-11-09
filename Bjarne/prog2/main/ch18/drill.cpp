#include <ch18/drill.h>
#include <iostream>
#include <sstream>

namespace my {

int ga[] {1,2,4,8,16,32,64,128,256,512};        // array 01

void fa(int ar[], int sz)                       // array 02
{
    int la[10];                                 // array 03.a
    for (int i = 0; i < 10; i++) {              // array 03.b
        la[i] = ga[i];
    }
    for (int i = 0; i < 10; ++i) {              // array 03.c
        std::cout << la[i] << ';';
    }
    int* p = new int[sz];                       // array 03.d
    int* pp = p;
    for (int i = 0; i < sz; ++i) {              // array 03.e
        *pp = ar[i];
        ++pp;
    }
    pp = p;
    for (int i = 0; i < sz; ++i) {              // array 03.f
        std::cout << *pp << ';';
        ++pp;
    }
    delete[] p;                                 // array 03.g
}

std::vector<int>
gv {1,2,4,8,16,32,64,128,256,512};              // vector 01

void fv(const std::vector<int>& v) {            // vector 02
    std::vector<int> lv;                        // vector 03.a
    for (auto e : gv) lv.push_back(e);          // vector 03.b
    for (auto e : lv) std::cout << e << ';';    // vector 03.c

    std::vector<int> lv2{v};                    // vector 03.d
    for (auto e : lv2) std::cout << e << ';';   // vector 03.e
}

}
