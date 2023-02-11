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

extern "C"
{
    TORCH_SYMBOL_EXPORT void MakeLowLevelApi(std::unique_ptr<Torch::LowLevelApi> &ptr)
    {
        ptr = Torch::MakeUniquePtr<Torch::OGLLowLevelApi>();
    }
}
