#ifndef UUIDS_GENERATOR_HPP
#define UUIDS_GENERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "common.h"
#include <chrono>
#include <random>

namespace uuids
{
    struct generator
    {
    public:
        static uint64_t generate_timestamp() { return _count_100_ns_intervals_from_15101582(); }
        static bool *generate_clock_sequence() { return _get_rnd_bits<14>(); }

    private:
        generator() = delete;
        inline static const constexpr time_t _base_date = 946684800;                      // 01/01/2000, 00:00:00
        inline static const constexpr uint64_t _base_date_cnt_100ns = 131659776000000000; // computed a priori, from 15/10/1582 00:00:00 to base date
        inline static std::default_random_engine _rnd_engine = std::default_random_engine();
        inline static std::uniform_int_distribution<int> _rnd_bool_distribution = std::uniform_int_distribution<int>(0, 1);

        static uint64_t _count_100_ns_intervals_from_15101582()
        {
            auto base_time = std::chrono::high_resolution_clock::from_time_t(_base_date);
            auto today_midnight = std::chrono::high_resolution_clock::now();
            auto diff_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(today_midnight - base_time).count();
            auto cnt_100ns = static_cast<uint64_t>(diff_ns / 100);
            return cnt_100ns + _base_date_cnt_100ns;
        }

        template <unsigned int n>
        static bool *_get_rnd_bits()
        {
            _rnd_engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

            bool *bits = new bool[n];
            for (unsigned int i = 0; i < n; ++i)
                bits[i] = (bool)_rnd_bool_distribution(_rnd_engine);
            return bits;
        }
    };
} // namespace uuids

#endif // UUIDS_GENERATOR_HPP
