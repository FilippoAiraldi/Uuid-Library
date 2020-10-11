#ifndef UUIDS_UUID_HPP
#define UUIDS_UUID_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "common.h"
#include "bits.h"
#include "generator.h"
#include <string>

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
        static uuid create()
        {
            uuid d;
            d._generate();
            return d;
        }

    public:
        uuid clone() { return uuid(*this); }
        std::string to_string() const { return _to_string(); }

        uuid &operator=(const uuid &other) { return _copy_bytes(other); }
        bool operator==(const uuid &other) { return _compare_bytes(*this, other); }
        bool operator!=(const uuid &other) { return !(*this == other); }

    private:
        uuid() = default;
        uuid(const uuid &) = default;

        void _generate()
        {
            _set_variant();
            _set_version();
            _set_timestamp();
            _set_clock_sequence();
        }

        void _set_variant()
        {
            // sets 2 msbs of the 8-th bytes
            bits::set_bit<uint8_t, 7>(_clock_seq_hi_and_reserved);   // msb0 = 1
            bits::clear_bit<uint8_t, 6>(_clock_seq_hi_and_reserved); // msb1 = 0
        }

        void _set_version()
        {
            // sets 4 msbs of the 6-th bytes
            bits::clear_bit<uint16_t, 15>(_time_hi_and_version); // msb0 = 0
            bits::clear_bit<uint16_t, 14>(_time_hi_and_version); // msb1 = 0
            bits::clear_bit<uint16_t, 13>(_time_hi_and_version); // msb2 = 0
            bits::set_bit<uint16_t, 12>(_time_hi_and_version);   // msb3 = 1
        }

        void _set_timestamp()
        {
            // sets the remaining 60 bits from bytes 0 to 7 (4 are taken by the uuid version)
            // get count of 100ns intervals from 15/10/1582
            uint64_t cnt = generator::generate_timestamp();

            for (uint8_t i = 0; i < 12; ++i)
            {
                bool b = bits::get_bit(cnt, i);
                bits::set_bit_to_value(_time_hi_and_version, i, b);
            }

            for (uint8_t i = 0; i < 16; ++i)
            {
                bool b = bits::get_bit(cnt, i + 12);
                bits::set_bit_to_value(_time_mid, i, b);
            }

            for (uint8_t i = 0; i < 32; ++i)
            {
                bool b = bits::get_bit(cnt, i + 28);
                bits::set_bit_to_value(_time_low, i, b);
            }
        }

        void _set_clock_sequence()
        {
            bool *rnd_bits = generator::generate_clock_sequence();

            bits::set_bit_to_value<uint8_t, 0>(_clock_seq_low, rnd_bits[0]);
            bits::set_bit_to_value<uint8_t, 1>(_clock_seq_low, rnd_bits[1]);
            bits::set_bit_to_value<uint8_t, 2>(_clock_seq_low, rnd_bits[2]);
            bits::set_bit_to_value<uint8_t, 3>(_clock_seq_low, rnd_bits[3]);
            bits::set_bit_to_value<uint8_t, 4>(_clock_seq_low, rnd_bits[4]);
            bits::set_bit_to_value<uint8_t, 5>(_clock_seq_low, rnd_bits[5]);
            bits::set_bit_to_value<uint8_t, 6>(_clock_seq_low, rnd_bits[6]);
            bits::set_bit_to_value<uint8_t, 7>(_clock_seq_low, rnd_bits[7]);

            bits::set_bit_to_value<uint8_t, 0>(_clock_seq_hi_and_reserved, rnd_bits[8]);
            bits::set_bit_to_value<uint8_t, 1>(_clock_seq_hi_and_reserved, rnd_bits[9]);
            bits::set_bit_to_value<uint8_t, 2>(_clock_seq_hi_and_reserved, rnd_bits[10]);
            bits::set_bit_to_value<uint8_t, 3>(_clock_seq_hi_and_reserved, rnd_bits[11]);

            bits::set_bit_to_value<uint8_t, 4>(_clock_seq_hi_and_reserved, rnd_bits[12]);
            bits::set_bit_to_value<uint8_t, 5>(_clock_seq_hi_and_reserved, rnd_bits[13]);

            delete rnd_bits;
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

    private:
        std::string _to_string() const
        {
            std::string s;

            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 3>(_time_low)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 2>(_time_low)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 1>(_time_low)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 0>(_time_low)));
            s.push_back('-');

            s.append(_byte_to_hexa_str(bits::get_byte<uint16_t, 1>(_time_mid)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint16_t, 0>(_time_mid)));
            s.push_back('-');

            s.append(_byte_to_hexa_str(bits::get_byte<uint16_t, 1>(_time_hi_and_version)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint16_t, 0>(_time_hi_and_version)));
            s.push_back('-');

            s.append(_byte_to_hexa_str(bits::get_byte<uint8_t, 0>(_clock_seq_hi_and_reserved)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint8_t, 0>(_clock_seq_low)));
            s.push_back('-');

            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 3>(_node_1)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 2>(_node_1)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 1>(_node_1)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint32_t, 0>(_node_1)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint16_t, 1>(_node_2)));
            s.append(_byte_to_hexa_str(bits::get_byte<uint16_t, 0>(_node_2)));
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

    std::ostream &operator<<(std::ostream &os, const uuid &d)
    {
        os << d.to_string();
        return os;
    }

} // namespace uuids

#endif // UUIDS_UUID_HPP
