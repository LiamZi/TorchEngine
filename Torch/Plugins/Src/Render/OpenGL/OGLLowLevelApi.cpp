#include <OpenGL/OGLLowLevelApi.hpp>


namespace Torch
{   
    OGLLowLevelApi::OGLLowLevelApi()
    {
    }

    std::wstring const &OGLLowLevelApi::Name() const
    {
        static std::wstring const name(L"OpenGL Render Api");
        return name;
    }
};


