/*
 * Project libime
 * Copyright (C) 2015  Xuetian Weng <wengxt@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LIBIME_UTILS_H
#define LIBIME_UTILS_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <arpa/inet.h>

namespace libime
{

template<typename T>
inline T load_data(const char* data) {
#if defined(__arm) || defined(__arm__) || defined(__sparc)
    T v;
    memcpy(&v, data, sizeof(v));
    return v;
#else
    return *reinterpret_cast<const T*>(data);
#endif
}

template<typename T>
inline void store_data(char* data, T v) {
#if defined(__arm) || defined(__arm__) || defined(__sparc)
    memcpy(data, &v, sizeof(v));
#else
    *reinterpret_cast<T*>(data) = v;
#endif
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), std::ostream&>::type marshall(std::ostream& out, T data)
{
    union {
        uint32_t i;
        T v;
    };
    static_assert(sizeof(T) == sizeof(uint32_t), "this function is only for 4 byte data");
    v = data;
    i = htonl(i);
    return out.write(reinterpret_cast<char*>(&i), sizeof(i));
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint8_t), std::ostream&>::type marshall(std::ostream& out, T data) {
    return out.write(reinterpret_cast<char*>(&data), sizeof(data));
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), std::ostream&>::type marshall(std::ostream& out, T data) {
    union {
        uint16_t i;
        T v;
    };
    static_assert(sizeof(T) == sizeof(uint16_t), "this function is only for 2 byte data");
    v = data;
    i = htons(i);
    return out.write(reinterpret_cast<char*>(&i), sizeof(i));
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), std::istream&>::type unmarshall(std::istream& in, T& data)
{
    union {
        uint32_t i;
        T v;
    };
    static_assert(sizeof(T) == sizeof(uint32_t), "this function is only for 4 byte data");
    if (in.read(reinterpret_cast<char*>(&i), sizeof(i))) {
        i = ntohl(i);
        data = v;
    }
    return in;
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint8_t), std::istream&>::type unmarshall(std::istream& in, T& data) {
    return in.read(reinterpret_cast<char*>(&data), sizeof(data));
}


template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), std::istream&>::type unmarshall(std::istream& in, T& data)
{
    union {
        uint16_t i;
        T v;
    };
    static_assert(sizeof(T) == sizeof(uint16_t), "this function is only for 4 byte data");
    if (in.read(reinterpret_cast<char*>(&i), sizeof(i))) {
        i = ntohs(i);
        data = v;
    }
    return in;
}

template<typename E>
void throw_if_fail(bool fail, E&& e) {
    if (fail) {
        throw e;
    }
}

inline void throw_if_io_fail(const std::ios& s) {
    throw_if_fail(!s, std::ios_base::failure("io fail"));
}

}

#endif // LIBIME_UTILS_H