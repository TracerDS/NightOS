#include <klibc/math.h>
#include <cstdint>
#include <bit>
#include <limits>

namespace detail {
    constexpr auto __TABLE_BITS__ = 7;
    constexpr auto __TABLE_SIZE__ = 1 << __TABLE_BITS__;

    // (double)((u64)2^(i/N) - i << 52-7)
    constexpr std::uint64_t __TABLE_idivN__[__TABLE_SIZE__] = {
        0x3FF0000000000000, 0x3FEFF63DA9FB3335,
        0x3FEFEC9A3E778061, 0x3FEFE315E86E7F85,
        0x3FEFD9B0D3158574, 0x3FEFD06B29DDF6DE,
        0x3FEFC74518759BC8, 0x3FEFBE3ECAC6F383,
        0x3FEFB5586CF9890F, 0x3FEFAC922B7247F7,
        0x3FEFA3EC32D3D1A2, 0x3FEF9B66AFFED31B,
        0x3FEF9301D0125B51, 0x3FEF8ABDC06C31CC,
        0x3FEF829AAEA92DE0, 0x3FEF7A98C8A58E51,
        0x3FEF72B83C7D517B, 0x3FEF6AF9388C8DEA,
        0x3FEF635BEB6FCB75, 0x3FEF5BE084045CD4,
        0x3FEF54873168B9AA, 0x3FEF4D5022FCD91D,
        0x3FEF463B88628CD6, 0x3FEF3F49917DDC96,
        0x3FEF387A6E756238, 0x3FEF31CE4FB2A63F,
        0x3FEF2B4565E27CDD, 0x3FEF24DFE1F56381,
        0x3FEF1E9DF51FDEE1, 0x3FEF187FD0DAD990,
        0x3FEF1285A6E4030B, 0x3FEF0CAFA93E2F56,
        0x3FEF06FE0A31B715, 0x3FEF0170FC4CD831,
        0x3FEEFC08B26416FF, 0x3FEEF6C55F929FF1,
        0x3FEEF1A7373AA9CB, 0x3FEEECAE6D05D866,
        0x3FEEE7DB34E59FF7, 0x3FEEE32DC313A8E5,
        0x3FEEDEA64C123422, 0x3FEEDA4504AC801C,
        0x3FEED60A21F72E2A, 0x3FEED1F5D950A897,
        0x3FEECE086061892D, 0x3FEECA41ED1D0057,
        0x3FEEC6A2B5C13CD0, 0x3FEEC32AF0D7D3DE,
        0x3FEEBFDAD5362A27, 0x3FEEBCB299FDDD0D,
        0x3FEEB9B2769D2CA7, 0x3FEEB6DAA2CF6642,
        0x3FEEB42B569D4F82, 0x3FEEB1A4CA5D920F,
        0x3FEEAF4736B527DA, 0x3FEEAD12D497C7FD,
        0x3FEEAB07DD485429, 0x3FEEA9268A5946B7,
        0x3FEEA76F15AD2148, 0x3FEEA5E1B976DC09,
        0x3FEEA47EB03A5585, 0x3FEEA34634CCC320,
        0x3FEEA23882552225, 0x3FEEA155D44CA973,
        0x3FEEA09E667F3BCD, 0x3FEEA012750BDABF,
        0x3FEE9FB23C651A2F, 0x3FEE9F7DF9519484,
        0x3FEE9F75E8EC5F74, 0x3FEE9F9A48A58174,
        0x3FEE9FEB564267C9, 0x3FEEA0694FDE5D3F,
        0x3FEEA11473EB0187, 0x3FEEA1ED0130C132,
        0x3FEEA2F336CF4E62, 0x3FEEA427543E1A12,
        0x3FEEA589994CCE13, 0x3FEEA71A4623C7AD,
        0x3FEEA8D99B4492ED, 0x3FEEAAC7D98A6699,
        0x3FEEACE5422AA0DB, 0x3FEEAF3216B5448C,
        0x3FEEB1AE99157736, 0x3FEEB45B0B91FFC6,
        0x3FEEB737B0CDC5E5, 0x3FEEBA44CBC8520F,
        0x3FEEBD829FDE4E50, 0x3FEEC0F170CA07BA,
        0x3FEEC49182A3F090, 0x3FEEC86319E32323,
        0x3FEECC667B5DE565, 0x3FEED09BEC4A2D33,
        0x3FEED503B23E255D, 0x3FEED99E1330B358,
        0x3FEEDE6B5579FDBF, 0x3FEEE36BBFD3F37A,
        0x3FEEE89F995AD3AD, 0x3FEEEE07298DB666,
        0x3FEEF3A2B84F15FB, 0x3FEEF9728DE5593A,
        0x3FEEFF76F2FB5E47, 0x3FEF05B030A1064A,
        0x3FEF0C1E904BC1D2, 0x3FEF12C25BD71E09,
        0x3FEF199BDD85529C, 0x3FEF20AB5FFFD07A,
    };

    template <typename T>
    struct FloatTraits;

    template<>
    struct FloatTraits<float> {
        using float_t = float;
        using int_t = std::int32_t;
        using uint_t = std::uint32_t;
        using limits = std::numeric_limits<float_t>;

        constexpr static std::uint8_t EXPONENT_WIDTH = 8;
        
        constexpr static std::uint8_t BIT_WIDTH = sizeof(float_t) * 8;
        constexpr static uint_t MANTISSA_WIDTH = static_cast<uint_t>(BIT_WIDTH) - EXPONENT_WIDTH - 1;
        constexpr static uint_t SIGN_MASK      = static_cast<uint_t>(1) << (BIT_WIDTH - 1);
        constexpr static uint_t MANTISSA_MASK  = (static_cast<uint_t>(1) << MANTISSA_WIDTH) - 1;
        constexpr static uint_t EXPONENT_MASK  = ~(SIGN_MASK | MANTISSA_MASK);

        constexpr static uint_t ToIntegral(float_t num) noexcept {
            return std::bit_cast<uint_t>(num);
        }
        constexpr static float_t ToFloat(uint_t num) noexcept {
            return std::bit_cast<float_t>(num);
        }
        
        constexpr static bool IsSign(float_t num) noexcept {
            return IsSign(ToIntegral(num));
        }
        constexpr static bool IsSign(uint_t num) noexcept {
            return GetSign(num) != 0;
        }
        constexpr static uint_t GetSign(float_t num) noexcept {
            return GetSign(ToIntegral(num));
        }
        constexpr static uint_t GetSign(uint_t num) noexcept {
            return num & SIGN_MASK;
        }
        constexpr static std::int8_t GetExponent(float_t num) noexcept {
            return GetExponent(ToIntegral(num));
        }
        constexpr static std::int8_t GetExponent(uint_t num) noexcept {
            return (num & EXPONENT_MASK) >> MANTISSA_WIDTH;
        }
        constexpr static uint_t GetMantissa(float_t num) noexcept {
            return GetMantissa(ToIntegral(num));
        }
        constexpr static uint_t GetMantissa(uint_t num) noexcept {
            return num & MANTISSA_MASK;
        }
    };

    template<>
    struct FloatTraits<double> {
        using float_t = double;
        using int_t = std::int64_t;
        using uint_t = std::uint64_t;
        using limits = std::numeric_limits<float_t>;

        constexpr static std::uint8_t EXPONENT_WIDTH = 11;
        
        constexpr static std::uint8_t BIT_WIDTH = sizeof(float_t) * 8;
        constexpr static uint_t MANTISSA_WIDTH = static_cast<uint_t>(BIT_WIDTH) - EXPONENT_WIDTH - 1;
        constexpr static uint_t SIGN_MASK      = static_cast<uint_t>(1) << (BIT_WIDTH - 1);
        constexpr static uint_t MANTISSA_MASK  = (static_cast<uint_t>(1) << MANTISSA_WIDTH) - 1;
        constexpr static uint_t EXPONENT_MASK  = ~(SIGN_MASK | MANTISSA_MASK);

        constexpr static uint_t ToIntegral(float_t num) noexcept {
            return std::bit_cast<uint_t>(num);
        }
        constexpr static float_t ToFloat(uint_t num) noexcept {
            return std::bit_cast<float_t>(num);
        }

        constexpr static bool IsSign(float_t num) noexcept {
            return IsSign(ToIntegral(num));
        }
        constexpr static bool IsSign(uint_t num) noexcept {
            return GetSign(num) != 0;
        }
        constexpr static uint_t GetSign(float_t num) noexcept {
            return GetSign(ToIntegral(num));
        }
        constexpr static uint_t GetSign(uint_t num) noexcept {
            return num & SIGN_MASK;
        }
        constexpr static std::int16_t GetExponent(float_t num) noexcept {
            return GetExponent(ToIntegral(num));
        }
        constexpr static std::int16_t GetExponent(uint_t num) noexcept {
            return (num & EXPONENT_MASK) >> MANTISSA_WIDTH;
        }
        constexpr static uint_t GetMantissa(float_t num) noexcept {
            return GetMantissa(ToIntegral(num));
        }
        constexpr static uint_t GetMantissa(uint_t num) noexcept {
            return num & MANTISSA_MASK;
        }
    };

    // round to nearest even
    // 1.5 * 2^52
    constexpr double SHIFT_UNSCALED = (1LL << FloatTraits<double>::MANTISSA_WIDTH) * 1.5f;
    // 1.5 * 2^45
    constexpr double SHIFT = SHIFT_UNSCALED / __TABLE_SIZE__;
}

namespace Constants {
    constexpr long double ln2 = 0.6931471805599453;
    constexpr long double one_third = 1.0L / 3.0L;
}

template <std::floating_point T>
T fmod_core(T x, T y) {
    using detail = detail::FloatTraits<T>;

    auto hx = detail::ToIntegral(x);
    auto hy = detail::ToIntegral(y);

    auto sx = hx & detail::SIGN_MASK;
    hx ^= sx;                 // |x|
    hy &= ~detail::SIGN_MASK; // |y|

    if (hx < hy) [[likely]] {
        // if y is NaN, return NaN
        if (hy > detail::EXPONENT_MASK) [[unlikely]] {
            return detail::limits::quiet_NaN();
        }
        return x;
    }

    auto ex = hx >> detail::MANTISSA_WIDTH;
    auto ey = hy >> detail::MANTISSA_WIDTH;
    auto exp_diff = ex - ey;

    if (
        ey < (detail::EXPONENT_MASK >> detail::MANTISSA_WIDTH) - detail::EXPONENT_WIDTH &&
        ey > detail::MANTISSA_WIDTH &&
        exp_diff <= detail::EXPONENT_WIDTH
    ) [[likely]] {
        auto mx = (hx << detail::EXPONENT_WIDTH) | detail::SIGN_MASK;
        auto my = (hy << detail::EXPONENT_WIDTH) | detail::SIGN_MASK;

        mx %= (my >> exp_diff);

        if (mx == 0) [[unlikely]] {
            return detail::ToFloat(sx);
        }
        auto shift = __builtin_clz(mx);
        ex -= shift + 1;
        mx <<= shift;
        mx = sx | (mx >> detail::EXPONENT_WIDTH);
        return detail::ToFloat(mx + (ex << detail::MANTISSA_WIDTH));
    }

    if (hy == 0 || hx >= detail::EXPONENT_MASK) [[unlikely]] {
        // if x is NaN, return NaN
        if (hx > detail::EXPONENT_MASK)
            return detail::limits::quiet_NaN();

        // If x is an infinity or y is zero, return NaN
        return detail::limits::quiet_NaN();
    }
    if (ex == 0) [[unlikely]] {
        return detail::ToFloat(sx | hx % hy);
    }

    auto mx = (hx & detail::MANTISSA_MASK) | (detail::MANTISSA_MASK + 1);
    auto my = (hy & detail::MANTISSA_MASK) | (detail::MANTISSA_MASK + 1);
    auto lead_zeros_my = detail::EXPONENT_WIDTH;

    ey--;    
    if (ey < 0) [[unlikely]] {
        my = hy;
        ey = 0;
        exp_diff--;
        lead_zeros_my = __builtin_clz(my);
    }

    auto tail_zeros_my = static_cast<unsigned long>(__builtin_ctz(my));
    auto sides_zeroes = static_cast<unsigned long>(lead_zeros_my + tail_zeros_my);

    auto right_shift = exp_diff < tail_zeros_my ? exp_diff : tail_zeros_my;
    my >>= right_shift;
    exp_diff -= right_shift;
    ey += right_shift;

    auto left_shift = exp_diff < detail::EXPONENT_WIDTH ? exp_diff : detail::EXPONENT_WIDTH;
    mx <<= left_shift;
    exp_diff -= left_shift;

    mx %= my;

    if (mx == 0) [[unlikely]] {
        return detail::ToFloat(sx);
    }

    if (exp_diff != 0) {
        // Multiplication with the inverse is faster than repeated modulo.
        auto inv_hy = std::numeric_limits<typename detail::uint_t>::max() / my;
        while (exp_diff > sides_zeroes) {
            exp_diff -= sides_zeroes;
            auto hd = (mx * inv_hy) >> (detail::BIT_WIDTH - sides_zeroes);
            mx <<= sides_zeroes;
            mx -= hd * my;
            while (mx > my) [[unlikely]] {
                mx -= my;
            }
        }
        auto hd = (mx * inv_hy) >> (detail::BIT_WIDTH - exp_diff);
        mx <<= exp_diff;
        mx -= hd * my;
        while (mx > my) [[unlikely]] {
            mx -= my;
        }
    }

    auto lz = __builtin_clz(mx) - detail::EXPONENT_WIDTH;
    mx <<= lz;
    ey -= lz;

    if (ey < 0 || mx == 0) [[unlikely]] {
        mx >>= -ey;
        ey = 0;
    }
    
    return detail::ToFloat(sx + mx + (ey << detail::MANTISSA_WIDTH));
}

float fabsf(float num) {
    return __builtin_fabsf(num);
}
double fabs(double num) {
    return __builtin_fabs(num);
}
long double fabsl(long double num) {
    return __builtin_fabsl(num);
}

float fmodf(float x, float y) {
    return fmod_core(x, y);
}
double fmod(double x, double y) {
    return fmod_core(x, y);
}

/*
long double fmodl(long double x, long double y) {
    return __builtin_fmodl(x, y);
}
float powf(float base, float exponent) {
    return __builtin_powf(base, exponent);
}
double pow(double base, double exponent) {
    return __builtin_pow(base, exponent);
}
long double powl(long double base, long double exponent) {
    return __builtin_powl(base, exponent);
}
*/

float expf(float num) {
    [[maybe_unused]] constexpr auto UPPER_LIMIT = 0x1.62e42ep6f;  // ln(FLT_MAX) in float representation
    [[maybe_unused]] constexpr auto LOWER_LIMIT = -0x1.9fe368p6f; // -ln(150.0f) in float representation
    return num;
}

float exp2f(float num) {
    using detailFloat = detail::FloatTraits<float>;
    using detailDouble = detail::FloatTraits<double>;

    constexpr auto INF = detailFloat::limits::infinity();
    
    double numDouble = num;
    auto bits = detailFloat::ToIntegral(num);
    auto absTop = (bits >> 20) & 0x7FF; // get top 12 bits

    // Get top 12 bits of 128.0f (max value before overflow)
    constexpr float absTop_128 = detailFloat::ToIntegral(128.0f) >> 20;

    if (absTop >= absTop_128) [[unlikely]] {
        // Check if infinity
        if (bits >= detailFloat::ToIntegral(-INF)) {
            return 0;
        }
        if (bits >= detailFloat::ToIntegral(INF)) {
            return INF;
        }

        // Check if NaN
        if (
            bits >= detailFloat::ToIntegral(detailFloat::limits::quiet_NaN()) ||
            bits >= detailFloat::ToIntegral(detailFloat::limits::signaling_NaN())
        ) {
            return detailFloat::limits::quiet_NaN();
        }

        // Check if too big
        if (num > 0.0f) {
            return INF; // inf
        }
        // Check if too small
        if (num < -150.0f) {
            return 0;
        }
    }

    // SHIFT = 2^53
    // x = k/N + r

    // kDouble = floor-to-int rounding
    auto kDouble = numDouble + detail::SHIFT;
    auto kInt = detailDouble::ToIntegral(kDouble);

    // kDouble = kDouble + SHIFT - SHIFT = k/N (rounded x)
    kDouble -= detail::SHIFT;

    // r = x - k/N
    auto r = numDouble - kDouble;

    // 2^x = 2^(k/N) * 2^r ~= s * (1 + C0 * r + C1 * r^2 + C2 * r^3)
    auto kn = detail::__TABLE_idivN__[kInt % detail::__TABLE_SIZE__];
    // Add exponent
    kn += kInt << (52 - detail::__TABLE_BITS__);
    double s = detailDouble::ToFloat(kn);

    // Calculate polynomial approximation

    double r2 = r * r;
    double r3 = r2 * r;
    constexpr double C0 = Constants::ln2;
    constexpr double C1 = C0 * Constants::ln2 * 0.5;
    constexpr double C2 = C1 * Constants::ln2 * Constants::one_third;

    return s * (1.0 + C0 * r + C1 * r2 + C2 * r3);
}

double exp2(double n) { return n; }

template <std::floating_point T>
constexpr T floorf_core(T x) noexcept {
    using detail = detail::FloatTraits<T>;

    constexpr typename detail::float_t SHIFT = static_cast<detail::uint_t>(1) << detail::MANTISSA_WIDTH;

    auto ix = detail::ToIntegral(x);

    // Already an integer, or NaN/Inf
    if ((ix & ~detail::SIGN_MASK) >= detail::ToIntegral(SHIFT)) {
        return x;
    }

    typename detail::float_t y = x;
    if (!detail::IsSign(ix)) {
        // Positive: round toward zero
        y += SHIFT;
        y -= SHIFT;
    } else {
        // Negative: round toward zero
        y -= SHIFT;
        y += SHIFT;
    }
    return y > x ? y - 1 : y;
}

float floorf(float x) {
    return floorf_core(x);
}
double floor(double x) {
    return floorf_core(x);
}

template <std::floating_point T>
constexpr T ceilf_core(T x) noexcept {
    using detail = detail::FloatTraits<double>;

    auto ix = detail::ToIntegral(x);
    auto exp = detail::GetExponent(x) - ((1 << (detail::EXPONENT_WIDTH - 1)) - 1);

    // NaN or Inf or already an integer
    if (exp >= static_cast<detail::int_t>(detail::MANTISSA_WIDTH))
        return x;

    // Absolute value less than 1
    if (exp < 0) {
        return x > 0 ? 1 : detail::ToFloat(ix & detail::SIGN_MASK);
    }

    detail::uint_t mask = (static_cast<detail::uint_t>(1) << (detail::MANTISSA_WIDTH - exp)) - 1;

    if ((ix & mask) == 0)
        return x; // already an integer

    if (x > 0.0f)
        ix += mask;

    return detail::ToFloat(ix & ~mask);
}

float ceilf(float num) { return ceilf_core(num); }
double ceil(double num) { return ceilf_core(num); }

float frexpf(float x, int* exp) {
    using detail = detail::FloatTraits<float>;

    auto ix = detail::ToIntegral(x);
    auto sign = ix & detail::SIGN_MASK;
    auto exponent = (ix & detail::EXPONENT_MASK) >> detail::MANTISSA_WIDTH;
    auto mantissa = ix & detail::MANTISSA_MASK;

    constexpr auto exponentBias = (1 << (detail::EXPONENT_WIDTH - 1)) - 1;

    if (exponent == 0) {
        // Zero or subnormal
        if (mantissa == 0) {
            *exp = 0;
            return x; // return signed zero
        }

        // Normalize subnormal
        int shift = 0;
        while ((mantissa & (detail::MANTISSA_MASK + 1)) == 0) {
            mantissa <<= 1;
            shift++;
        }

        exponent = 1; // For subnormals, true exponent is 1 - 127
        mantissa &= detail::MANTISSA_MASK; // Remove implicit 1
        exponent -= shift;
    } else {
        if (exponent == 255) {
            // NaN or Inf
            *exp = 0;
            return x;
        }
    }
    *exp = static_cast<int>(exponent) - exponentBias + 1;

    // Reconstruct float with adjusted exponent of 0 (bias 126)
    return detail::ToFloat(sign | ((exponentBias - 1) << detail::MANTISSA_WIDTH) | mantissa);
}

