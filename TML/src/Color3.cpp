#include <TML/TML.hpp>
#include <TML/Color3.hpp>
#include <TML/Color4.hpp>


namespace Torch
{
    Color3::Color3(float red, float green, float blue)
        : _r{red}
        , _g{green}
        , _b{blue}
    {

    }

    Color3::Color3(const Color3 &other)
    {

    }

    Color3::Color3(Color3 &&other)
    {
    }

    Color3 &Color3::operator=(const Color3 &other) = default;

    Color3 &Color3::operator=(const Color4 &other)
    {
        _r = other._r;
        _b = other._b;
        _g = other._g;
        return *this;
    }

    Color3 &Color3::operator=(Color3 &&other) = default;
    
    Color3::~Color3() = default;
};
