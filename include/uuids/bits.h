#ifndef UUIDS_ALGORITHMS_HPP
#define UUIDS_ALGORITHMS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "common.h"

/*
All of the bits and bytes manipulation methods consider the
0-th bit as the lsb (i.e., right-hand, zero-based).
*/

namespace uuids
{
    struct bits
    {
    public:
        template <typename T, uint8_t n>
        static void set_bit(T &x) // sets n-th bit to 1
        {
            static_assert(n < 8 * sizeof(T), "bit outside range");
            x |= true << n;
        }

        template <typename T>
        static void set_bit(T &x, uint8_t n) // sets n-th bit to 1
        {
            if (n >= 8 * sizeof(T))
                throw std::invalid_argument("bit outside range");
            x |= true << n;
        }

        template <typename T, uint8_t n>
        static void clear_bit(T &x) // sets n-th bit to 0
        {
            static_assert(n < 8 * sizeof(T), "bit outside range");
            x &= ~(true << n);
        }

        template <typename T>
        static void clear_bit(T &x, uint8_t n) // sets n-th bit to 0
        {
            if (n >= 8 * sizeof(T))
                throw std::invalid_argument("bit outside range");
            x &= ~(true << n);
        }

        template <typename T, uint8_t n>
        static void set_bit_to_value(T &x, bool val) // sets n-th bit to the given val
        {
            static_assert(n < 8 * sizeof(T), "bit outside range");
            if (val)
                set_bit<T, n>(x);
            else
                clear_bit<T, n>(x);
        }

        template <typename T>
        static void set_bit_to_value(T &x, uint8_t n, bool val) // sets n-th bit to the given val
        {
            if (n >= 8 * sizeof(T))
                throw std::invalid_argument("bit outside range");
            if (val)
                set_bit<T>(x, n);
            else
                clear_bit<T>(x, n);
        }

        template <typename T, uint8_t n>
        static bool get_bit(T &x) // gets n-th bit
        {
            if (n < 8 * sizeof(T))
                return (x >> n) & true;
            return false;
        }

        template <typename T>
        static bool get_bit(T &x, uint8_t n) // gets n-th bit
        {
            if (n < 8 * sizeof(T))
                return (x >> n) & true;
            return false;
        }

        template <typename T, uint8_t n>
        static uint8_t get_byte(const T &x) // gets n-th byte
        {
            static_assert(n < sizeof(T), "byte outside range");
            return ((x >> (8 * n)) & _byte_mask);
        }

    private:
        bits() = delete;
        inline static const constexpr uint8_t _byte_mask = 255; // byte with all 1s
    };
} // namespace uuids

#endif // UUIDS_ALGORITHMS_HPP
