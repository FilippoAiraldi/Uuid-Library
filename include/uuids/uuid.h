#ifndef UUIDS_UUID_HPP
#define UUIDS_UUID_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "common.h"
#include "algorithms.h"
#include <string>
/* #include <ios>
#include <iomanip>
#include <sstream> */

/* --------------------------------------------------------------------------
Uuid layout:
x x x x x x x x - x x x x -M x x x -N x x x - x x x x x x x x x x x x (hexas)
0   1   2   3     4   5    6   7    8   9     10  11  12  13  14  15  (bytes)

The 4 bits of M (6-th byte) define the uuid version.
The first 3 msbs of N (8-th byte) define the uuid variant.
Bytes are given specific group, listed below:
    time_low                   0-3
    time_mid                   4-5
    time_hi_and_version        6-7
    clock_seq_hi_and_reserved  8
    clock_seq_low              9
    node                       10-15
-------------------------------------------------------------------------- */

namespace uuids
{
    struct uuid final
    {
    public:
        virtual ~uuid() {}
        static uuid empty() { return uuid(); }
        static uuid create(algorithms::types type = algorithms::types::time_based)
        {
            return uuid(type);
        }

    public:
        uuid clone() { return uuid(*this); }
        uuid &operator=(const uuid &other) { return _copy_bytes(other); }
        bool operator==(const uuid &other) { return _compare_bytes(*this, other); }
        bool operator!=(const uuid &other) { return !(*this == other); }
        std::string to_string() const { return _to_string(); }

    private:
        uuid() = default;
        uuid(const uuid &) = default;
        uuid(algorithms::types type)
        {
            _set_variant();
            _set_version(type);
        }

    private:
        uint32_t _time_low{};                 // 0-3
        uint16_t _time_mid{};                 // 4-5
        uint16_t _time_hi_and_version{};      // 6-7
        uint8_t _clock_seq_hi_and_reserved{}; // 8
        uint8_t _clock_seq_low{};             // 9
        uint32_t _node_1{};                   // 10-13
        uint16_t _node_2{};                   // 14-15

        uuid &_copy_bytes(const uuid &other)
        {
            // no move constructor or assignment
            // since fields are of very simple types
            _time_low = other._time_low;
            _time_mid = other._time_mid;
            _time_hi_and_version = other._time_hi_and_version;
            _clock_seq_hi_and_reserved = other._clock_seq_hi_and_reserved;
            _clock_seq_low = other._clock_seq_low;
            _node_1 = other._node_1;
            _node_2 = other._node_2;
            return *this;
        }

        bool _compare_bytes(const uuid &lhs, const uuid &rhs)
        {
            return lhs._time_low == rhs._time_low &&
                   lhs._time_mid == rhs._time_mid &&
                   lhs._time_hi_and_version == rhs._time_hi_and_version &&
                   lhs._clock_seq_hi_and_reserved == rhs._clock_seq_hi_and_reserved &&
                   lhs._clock_seq_low == rhs._clock_seq_low &&
                   lhs._node_1 == rhs._node_1 &&
                   lhs._node_2 == rhs._node_2;
        }

        void _set_variant()
        {
            algorithms::set_bit(_clock_seq_hi_and_reserved, 7);   // msb0 = 1
            algorithms::clear_bit(_clock_seq_hi_and_reserved, 6); // msb1 = 0
        }

        void _set_version(algorithms::types type)
        {
            switch (type)
            {
            case algorithms::types::time_based:
                algorithms::clear_bit(_time_hi_and_version, 15); // msb0 = 0
                algorithms::clear_bit(_time_hi_and_version, 14); // msb1 = 0
                algorithms::clear_bit(_time_hi_and_version, 13); // msb2 = 0
                algorithms::set_bit(_time_hi_and_version, 12);   // msb3 = 1
                break;
            case algorithms::types::name_based:
                algorithms::clear_bit(_time_hi_and_version, 15); // msb0 = 0
                algorithms::clear_bit(_time_hi_and_version, 14); // msb1 = 0
                algorithms::set_bit(_time_hi_and_version, 13);   // msb2 = 1
                algorithms::set_bit(_time_hi_and_version, 12);   // msb3 = 1
                break;
            default:
                throw std::invalid_argument("invalid uuid algorithm");
            }
        }

        std::string _to_string() const
        {
            std::string s;
            uint8_t byte_mask = 255;

            s.append(_byte_to_hexa_str((_time_low >> 24) & byte_mask));
            s.append(_byte_to_hexa_str((_time_low >> 16) & byte_mask));
            s.append(_byte_to_hexa_str((_time_low >> 8) & byte_mask));
            s.append(_byte_to_hexa_str(_time_low & byte_mask));

            s.push_back('-');

            s.append(_byte_to_hexa_str((_time_mid >> 8) & byte_mask));
            s.append(_byte_to_hexa_str(_time_mid & byte_mask));

            s.push_back('-');

            s.append(_byte_to_hexa_str((_time_hi_and_version >> 8) & byte_mask));
            s.append(_byte_to_hexa_str(_time_hi_and_version & byte_mask));

            s.push_back('-');

            s.append(_byte_to_hexa_str(_clock_seq_hi_and_reserved));
            s.append(_byte_to_hexa_str(_clock_seq_low));

            s.push_back('-');

            s.append(_byte_to_hexa_str((_node_1 >> 24) & byte_mask));
            s.append(_byte_to_hexa_str((_node_1 >> 16) & byte_mask));
            s.append(_byte_to_hexa_str((_node_1 >> 8) & byte_mask));
            s.append(_byte_to_hexa_str(_node_1 & byte_mask));
            s.append(_byte_to_hexa_str((_node_2 >> 8) & byte_mask));
            s.append(_byte_to_hexa_str(_node_2 & byte_mask));

            return s;
        }

        static std::string _byte_to_hexa_str(uint8_t n)
        {
            uint8_t lsb_4_bits = n & 15; // 15 = ~(1 << 4)
            uint8_t msb_4_bits = (n >> 4) & 15;

            std::string s;
            s.push_back(_4_bits_to_hexa_char(msb_4_bits));
            s.push_back(_4_bits_to_hexa_char(lsb_4_bits));
            return s;
        }

        static char _4_bits_to_hexa_char(uint8_t n)
        {
            if (n < 10)
                return (char)n + '0';
            return (char)(n - 10) + 'a';
        }
    };
} // namespace uuids

#endif // UUIDS_UUID_HPP
