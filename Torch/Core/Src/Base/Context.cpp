//#include <Torch/Torch.hpp>

#include <fstream>
#include <mutex>
#include <sstream>

#if defined(TORCH_PLATFORM_WINDOWS)
#include <windows.h>
#if defined(TORCH_PLATFORM_WINDOWS_DESKTOP)
#endif
#endif

#include <Torch/Context.hpp>


namespace Torch
{
    Context::Context()
    {

    }

    Context::~Context()
    {
        
    }
};
