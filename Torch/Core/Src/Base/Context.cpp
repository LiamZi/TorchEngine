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

#include <Torch/Interfaces/Context.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>

namespace
{
    std::mutex singleton_mutex;
}

namespace Torch
{
    std::unique_ptr<Context> Context::_context_instance;

    Context::Context()
    : App{this, &Context::getApp, &Context::setApp}
    , AppValid{this, &Context::isAppValid}
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

    void Torch::Context::setApp(const App3DFrameworkPtr &app)
    {
        _app = app;
    }

    App3DFrameworkPtr &Context::getApp()
    {
        return _app;
    }

    bool Torch::Context::isAppValid() const
    {
        return _app != nullptr;
    }
};
