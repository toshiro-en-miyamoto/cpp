#include <ch19/vector_ptr.h>
#include <boost/ut.hpp>
#include <my/hijack.h>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "vector ptr"_test = [] {
        given("default constructor") = [] {
            my::hijack hj {std::cout};
            when("instanciated") = [] {
                ch19::vector<int> v;
                expect(v.capacity() == 0_ul);
                expect(v.size() == 0_ul);
            };
            expect(hj.release() ==
                "new  0\n"
                "del  0\n"
            );
        };
        given("size constructor") = [] {
            my::hijack hj1 {std::cout};
            when("instanciated with implicit default") = [] {
                ch19::vector<int> v(2);
                expect(v.capacity() == 2_ul);
                expect(v.size() == 2_ul);

                int sum = 0;
                for (auto i = 0; i < v.size(); ++i) {
                    sum += v[i];
                }
                expect(sum == 0_i);
            };
            expect(hj1.release() ==
                "size 2\n"
                "del  2\n"
            );
            my::hijack hj2 {std::cout};
            when("instanciated with implicit default") = [] {
                ch19::vector<int> v(3, 4);
                expect(v.capacity() == 3_ul);
                expect(v.size() == 3_ul);

                int sum = 0;
                for (auto i = 0; i < v.size(); ++i) {
                    sum += v[i];
                }
                expect(sum == 12_i);
            };
            expect(hj2.release() ==
                "size 3\n"
                "del  3\n"
            );
        };
        given("ctor taking initializer") = [] {
            my::hijack hj {std::cout};
            when("instanciated with list") = [] {
                ch19::vector v {1, 2, 3};
                expect(v.capacity() == 3_i);
                expect(v.size() == 3_i);

                int sum = 0;
                for (auto i = 0; i < v.size(); ++i) {
                    sum += v[i];
                }
                expect(sum == 6_i);
            };
            expect(hj.release() ==
                "list 3\n"
                "del  3\n"
            );
        };
    };
}
