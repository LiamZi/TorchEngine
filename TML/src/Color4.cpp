#include <TML/TML.hpp>
#include <TML/Color4.hpp>
#include <TML/Color3.hpp>

namespace Torch
{
    Color4::Color4(float red, float green, float blue, float alpha)
        : _r{red}
        , _g{green}
        , _b{blue}
        , _a{alpha}
    {
    }

    Color4::Color4(const Color3 &other)
        : _r{other._r}
        , _g{other._g}
        , _b{other._b}
        , _a{1.0f}
    {
    }

    Color4::Color4(Color3 &&other)
    {
        _r = other._r;
        _g = other._g;
        _b = other._b;
        _a = 1.0f;
    }

    Color4::Color4(const Color4 &other) = default;

    Color4::Color4(Color4 &&other) = default;

    Color4 &Color4::operator=(const Color4 &other) = default;

    Color4 &Color4::operator=(const Color3 &other) 
    {
        _r = other._r;
        _g = other._g;
        _b = other._b;
        _a = 1.f;

        return *this;
    }

    Color4 &Color4::operator=(const Color3 &&other)
    {
        _r = std::move(other._r);
        _g = std::move(other._g);
        _b = std::move(other._b);
        _a = 1.f;

        return *this;
    }
    Color4 &Color4::operator=(Color4 &&other) = default;

    Color4::~Color4() = default;
};