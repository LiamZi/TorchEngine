#include <Torch/Torch.hpp>
#include <TML/Util.hpp>

#include <fstream>
#include <mutex>
#include <sstream>

#if defined(TORCH_PLATFORM_WINDOWS)
#include <windows.h>
#if defined(TORCH_PLATFORM_WINDOWS_DESKTOP)
#endif
#endif

#include <Torch/Context.hpp>

namespace
{
    std::mutex singleton_mutex;
}

namespace Torch
{
    std::unique_ptr<Context> Context::_context_instance;

    Context::Context()
    {

    }

    Context::~Context()
    {
        
    }

    Context &Torch::Context::Instance()
    {
        if(!_context_instance)
        {
            std::lock_guard<std::mutex> lock(singleton_mutex);
            if(!_context_instance)
            {
                _context_instance = MakeUniquePtr<Context>();
            }
        }

        return *_context_instance;
    }

    void Context::Destroy()
    {
        std::lock_guard<std::mutex> lock(singleton_mutex);
        if(_context_instance)
        {
            _context_instance->DestoryAll();
            _context_instance.reset();
        }
    }

    void Context::Suspend()
    {
        
    }

    void Context::Resume()
    {
    }

    void Torch::Context::DestoryAll()
    {
    }
};
