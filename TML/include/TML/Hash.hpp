#ifndef __TML_HASH_HPP__
#define __TML_HASH_HPP__

#pragma once

#include <span>
#include <string>
#include <string_view>


namespace Torch
{
#define MAGIC_NUM 0x9e3779b9


#ifdef TORCH_COMPILER_MSVC
#pragma warning(disable: 4307)
#endif

    size_t constexpr HashImpl(char const *str, size_t seed) noexcept
    {
        return 0 == *str ? seed : HashImpl(str + 1, seed ^ (static_cast<size_t>(*str) + MAGIC_NUM + (seed << 6) + (seed >> 2)));
    }

#if defined(TORCH_COMPILER_MSVC) && (_MSC_VER < 1914)
    template<size_t T>
    struct  EnsureConst
    {   
        static size_t constexpr value = T;
    };

    #define _HASH(x) (EnsureConst < HashImpl(x, 0)>::value)
#else
    #define _HASH(x) (HashImpl(x, 0))
#endif

    template<typename T>
    inline void HashCombineImpl(T &seed, T value) noexcept
    {
        seed ^= value + MAGIC_NUM + (seed << 6) + (seed >> 2);
    }

    inline size_t _Hash(const char *str) noexcept
    {
        size_t seed = 0;
        while(*str != 0)
        {
            HashCombineImpl(seed, static_cast<size_t>(*str));
            ++str;
        }
        return seed;
    }

#undef MAGIC_NUM

    template<typename T>
    inline size_t HashValue(T v) noexcept
    {
        return static_cast<size_t>(v);
    }

    template<typename T>
    inline size_t HashValue(T* v) noexcept
    {
        return static_cast<size_t>(reinterpret_cast<ptrdiff_t>(v));
    }

    template<typename T>
    inline void HashCombine(size_t &seed, const T &v) noexcept
    {
        return HashCombineImpl(seed, HashValue(v));
    }

    template<typename T>
    inline void HashRange(size_t &seed, T first, T last) noexcept
    {
        for(; first != last; ++first)
        {
            HashCombine(seed, *first);
        }
    }

    template<typename T>
    inline size_t HashRange(T first, T last) noexcept
    {
        size_t seed = 0;
        HashRange(seed, first, last);
        return seed;
    }

    template<typename T>
    inline size_t HashValue(std::span<T const> s) noexcept
    {
        return HashRange(s.begin(), s.end());
    } 

    template<typename T>
    inline size_t HashValue(std::basic_string_view<T> sv) noexcept
    {
        return HashRange(sv.begin(), sv.end());
    }

    template<typename T>
	inline size_t HashValue(std::basic_string<T> const& str) noexcept
	{
		return HashRange(str.begin(), str.end());
	}
};

#endif