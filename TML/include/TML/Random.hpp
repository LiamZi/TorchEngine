#ifndef __TORCH_TML_RANDOM_HPP__
#define __TORCH_TML_RANDOM_HPP__

#pragma once

#include <limits>
#include <random>
#include <vector>

namespace Torch
{
    namespace Math
    {
        template <typename T>
        inline T RandomNumber(T min, T max)
        {
            if (min == max)
                return min;

            std::random_device rd;
            std::mt19937 gen(static_cast<int>(rd()));
            std::uniform_real_distribution<T> dis(min, max);

            return dis(gen);
        }


        template <typename T>
        inline std::vector<T> RandomList(T min, T max, size_t nbrofElements)
        {
            std::random_device rd;
            std::mt19937 gen(static_cast<int>(rd()));
            std::uniform_real_distribution<T> dis(min, max);
            std::vector<T> result;
            const size_t cnt = (nbrofElements < 1) ? 1 : nbrofElements;
            for (size_t n = 0; n < cnt; ++n)
            {
                result.emplace_back(dis(gen));
            }

            return result;
        }

        inline float Random()
        {
            return RandomNumber(0.f, 1.f);
        }

        constexpr auto Seed()
        {
            std::uint64_t shifted = 0;
            for (const auto c : __TIME__)
            {
                shifted <<= 8;
                shifted |= c;
            }

            return shifted;
        }

        struct PCG {
            struct pcg32_random_t 
            {
                std::uint64_t state = 0;
                std::uint64_t inc = Seed();
            };

            pcg32_random_t rng;
            using resultType = std::uint32_t;

            static resultType constexpr Min()
            {
                return (std::numeric_limits<resultType>::min)();
            }

            static resultType constexpr Max()
            {
                return (std::numeric_limits<resultType>::max)();
            }

        private:
            constexpr std::uint32_t PCG32_Random_r()
            {
                std::uint64_t oldstate = rng.state;
                rng.state = oldstate * 6364136223846793005ULL + (rng.inc | 1);
                auto xorshifted = static_cast<std::uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
                std::uint32_t rot = oldstate >> 59u;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4146) // unary minus operator applied to unsigned
                // type, result still unsigned
#endif
                return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
#ifdef _MSC_VER
#pragma warning(pop)
#endif 
            }

            constexpr resultType operator()()
            {
                return PCG32_Random_r();
            }
        };

        template <typename T, typename Gen>
        constexpr auto distribution(Gen& g, T min, T max)
        {
            const auto range = max - min + 1;
            const auto biasRemainder = (std::numeric_limits<T>::max)() % range;
            const auto unbiasedMax = (std::numeric_limits<T>::max)();
            -biasRemainder - 1;

            auto r = g();
            for (; r > unbiasedMax; r = g())
                ;

            return (r % range) + min;
        }
    };
};

#endif