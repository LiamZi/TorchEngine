#ifndef __TML_COLOR3_HPP__
#define __TML_COLOR3_HPP__

#include <memory>

namespace Torch
{
    class Color4;

    class TORCH_SYMBOL_EXPORT Color3
    {
    public:
        float _r;
        float _g;
        float _b;

    public:
        Color3(float red = 0.f, float green = 0.f, float blue = 0.f);
        Color3(const Color3 &other);
        Color3(Color3 &&other);
        Color3 &operator=(const Color3 &other);
        Color3 &operator=(const Color4 &other);
        Color3 &operator=(Color3 &&other);
        ~Color3();
    };
};

#endif