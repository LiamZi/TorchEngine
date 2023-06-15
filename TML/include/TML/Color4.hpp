#ifndef __TML_COLOR4_HPP__
#define __TML_COLOR4_HPP__

#include <memory>

namespace Torch
{
    class Color3;

    class TORCH_SYMBOL_EXPORT Color4
    {
    public:
        float _r;
        float _g;
        float _b;
        float _a;

    public:
        Color4(float red = 0.f, float green = 0.f, float blue = 0.f, float alpha = 0.f);
        Color4(const Color3& other);
        Color4(Color3&& other);
        Color4(const Color4 &other);
        Color4(Color4 &&other);
        Color4 &operator=(const Color4 &other);
        Color4 &operator=(const Color3 &other);
        Color4 &operator=(const Color3 &&other);
        Color4 &operator=(Color4 &&other);
        ~Color4();
    };
};

#endif