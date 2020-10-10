#ifndef UUIDS_ALGORITHMS_HPP
#define UUIDS_ALGORITHMS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "common.h"

namespace uuids
{

    struct algorithms
    {
    public:
        enum types
        {
            time_based,
            name_based
        };

        template <typename T>
        static void set_bit(T &x, uint8_t n)
        {
            if (n < 8 * sizeof(T))
                x |= true << n;
        }

        template <typename T>
        static void clear_bit(T &x, uint8_t n)
        {
            if (n < 8 * sizeof(T))
                x &= ~(true << n);
        }

        template <typename T>
        static bool check_bit(T &x, uint8_t n)
        {
            if (n < 8 * sizeof(T))
                return (x >> n) & true;
            return false;
        }

    private:
        algorithms() = delete;
    };
} // namespace uuids

#endif // UUIDS_ALGORITHMS_HPP
