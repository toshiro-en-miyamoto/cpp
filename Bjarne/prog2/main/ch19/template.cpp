#include <concepts>
#include <bit>

// rounds down by the most significant bits
template<std::unsigned_integral T, std::size_t bits>
inline constexpr T round_down(T a) noexcept
{
    return (a >> bits) << bits;
}

static_assert( 0 == round_down<unsigned char, 3>( 0));
static_assert( 0 == round_down<unsigned char, 3>( 1));
static_assert( 0 == round_down<unsigned char, 3>( 7));
static_assert( 8 == round_down<unsigned char, 3>( 8));
static_assert( 8 == round_down<unsigned char, 3>( 9));
static_assert( 8 == round_down<unsigned char, 3>(15));
static_assert(16 == round_down<unsigned char, 3>(16));
static_assert(16 == round_down<unsigned char, 3>(17));
static_assert(16 == round_down<unsigned char, 3>(23));
static_assert(24 == round_down<unsigned char, 3>(24));
static_assert(24 == round_down<unsigned char, 3>(25));
static_assert(24 == round_down<unsigned char, 3>(31));
static_assert(32 == round_down<unsigned char, 3>(32));

// std::bit_floor()
// returns the largest integral power of two
// not greater than the given value
static_assert( 0 == std::bit_floor<unsigned char>( 0));
static_assert( 1 == std::bit_floor<unsigned char>( 1));
static_assert( 2 == std::bit_floor<unsigned char>( 2));
static_assert( 2 == std::bit_floor<unsigned char>( 3));
static_assert( 4 == std::bit_floor<unsigned char>( 4));
static_assert( 4 == std::bit_floor<unsigned char>( 5));
static_assert( 4 == std::bit_floor<unsigned char>( 7));
static_assert( 8 == std::bit_floor<unsigned char>( 8));
static_assert( 8 == std::bit_floor<unsigned char>( 9));
static_assert( 8 == std::bit_floor<unsigned char>(15));
static_assert(16 == std::bit_floor<unsigned char>(16));
static_assert(16 == std::bit_floor<unsigned char>(17));
static_assert(16 == std::bit_floor<unsigned char>(31));
static_assert(32 == std::bit_floor<unsigned char>(32));

// rounds up by the least significant bits
template<std::unsigned_integral T, std::size_t bits>
inline constexpr T round_up(T a) noexcept
{
    T round_up_seed = (1 << bits) - 1;
    return ((a + round_up_seed) >> bits) << bits; 
}

static_assert( 0 == round_up<unsigned char, 3>( 0));
static_assert( 8 == round_up<unsigned char, 3>( 1));
static_assert( 8 == round_up<unsigned char, 3>( 7));
static_assert( 8 == round_up<unsigned char, 3>( 8));
static_assert(16 == round_up<unsigned char, 3>( 9));
static_assert(16 == round_up<unsigned char, 3>(15));
static_assert(16 == round_up<unsigned char, 3>(16));
static_assert(24 == round_up<unsigned char, 3>(17));
static_assert(24 == round_up<unsigned char, 3>(23));
static_assert(24 == round_up<unsigned char, 3>(24));
static_assert(32 == round_up<unsigned char, 3>(25));
static_assert(32 == round_up<unsigned char, 3>(31));
static_assert(32 == round_up<unsigned char, 3>(32));

// std::bit_ceil()
// returns the smallest integral power of two
// not less than the given value
static_assert( 1 == std::bit_ceil<unsigned char>( 0));
static_assert( 1 == std::bit_ceil<unsigned char>( 1));
static_assert( 2 == std::bit_ceil<unsigned char>( 2));
static_assert( 4 == std::bit_ceil<unsigned char>( 3));
static_assert( 4 == std::bit_ceil<unsigned char>( 4));
static_assert( 8 == std::bit_ceil<unsigned char>( 5));
static_assert( 8 == std::bit_ceil<unsigned char>( 7));
static_assert( 8 == std::bit_ceil<unsigned char>( 8));
static_assert(16 == std::bit_ceil<unsigned char>( 9));
static_assert(16 == std::bit_ceil<unsigned char>(15));
static_assert(16 == std::bit_ceil<unsigned char>(16));
static_assert(32 == std::bit_ceil<unsigned char>(17));
static_assert(32 == std::bit_ceil<unsigned char>(31));
static_assert(32 == std::bit_ceil<unsigned char>(32));


int main()
{
}
