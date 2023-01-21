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
#include <TML/Thread.hpp>
#include <Torch/Interfaces/Engine.hpp>

namespace
{
    std::mutex singleton_mutex;
}

namespace Torch
{
    std::unique_ptr<Context> Context::_context_instance;

    Context::Context()
        : _app{ nullptr }
        , AppValid{ this, &Context::isAppValid }
    {
        _tp_instance = MakeUniquePtr<ThreadPool>(1, 8);
    }

    Context::~Context()
    {

    }

    Context& Torch::Context::Instance()
    {
        if (!_context_instance)
        {
            std::lock_guard<std::mutex> lock(singleton_mutex);
            if (!_context_instance)
            {
                _context_instance = MakeUniquePtr<Context>();
            }
        }

        return *_context_instance;
    }

    void Context::Destroy()
    {
        std::lock_guard<std::mutex> lock(singleton_mutex);
        if (_context_instance)
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

    void Context::LoadRenderEngine(std::string const& name)
    {
        _render_engine.reset();

        _render_loader.Free();

        //std::string render_path = ResourceLoader::Instance().Locate("Render");
    }

    void Context::DestoryAll()
    {
    }

    ThreadPool &Context::ThreadPools()
    {
        return *_tp_instance;
    }

    void Context::setApp(App3DFramework *app)
    {
        _app = app;
    }

    App3DFramework *Context::getApp()
    {
        return _app;
    }

    void Context::setConfig(const ContextCfg &cfg)
    {
        _cfg = cfg;
    }

    ContextCfg &Context::getConfig()
    {
        return _cfg;
    }

    bool Context::isAppValid() const
    {
        return _app != nullptr;
    }
};
