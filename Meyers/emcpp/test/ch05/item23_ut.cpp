#include <type_traits>

void remove_ref()
{
    static_assert( std::is_same_v <
        int,
        std::remove_reference_t<int>
    >);
    static_assert( std::is_same_v <
        int,
        std::remove_reference_t<int&>
    >);
    static_assert( std::is_same_v <
        int,
        std::remove_reference_t<int&&>
    >);
    static_assert( std::is_same_v <
        const int,
        std::remove_reference_t<const int>
    >);
    static_assert( std::is_same_v <
        const int,
        std::remove_reference_t<const int&>
    >);
    static_assert( std::is_same_v <
        const int,
        std::remove_reference_t<const int&&>
    >);
}

int main()
{}
