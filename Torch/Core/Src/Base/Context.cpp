#include <Torch/Torch.hpp>
#include <TML/Util.hpp>

#include <fstream>
#include <mutex>
#include <sstream>
#include <iostream>

#if defined(TORCH_PLATFORM_WINDOWS)
#include <windows.h>
#if defined(TORCH_PLATFORM_WINDOWS_DESKTOP)
#endif
#endif

#include <Torch/Interfaces/Context.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>
#include <TML/Thread.hpp>
#include <Torch/Interfaces/Engine.hpp>
#include <Torch/Resources/ResourceLoader.hpp>
#include <Torch/Interfaces/LowLevelApi.hpp>

#define TORCH_DLL_PREFIX DLL_PREFIX TML_STRINGIZE(TORCH_NAME)

namespace
{
    std::mutex singleton_mutex;
}

namespace Torch
{
    std::unique_ptr<Context> Context::_context_instance;

    using MakeRenderEngineFunc = void (*)(std::unique_ptr<Engine> &ptr);

    Context::Context()
        : _app{ nullptr }
        , AppValid{ this, &Context::isAppValid }
    {
        _tp_instance = MakeUniquePtr<ThreadPool>(1, 8);
        _cfg._render_engine_name = "OpenGL";
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
        
        std::string engine_path = ResourceLoader::Instance().Locate("Render");
        std::string fn = TORCH_DLL_PREFIX"_Engine_" + name + DLL_SUFFIX;

        std::string path = engine_path + "/" + fn;
        _render_loader.Load(ResourceLoader::Instance().Locate(path));

        auto *mrf = reinterpret_cast<MakeRenderEngineFunc>(_render_loader.GetProcAddress("MakeRenderEngine"));
        if(mrf != nullptr)
        {
            mrf(_render_engine);
        }
        else
        {
            std::cout << "Loading " << path << " failed " << std::endl;
            _render_loader.Free();
        }
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

    Engine &Context::Engine()
    {
        if(!_render_engine)
        {
            std::lock_guard<std::mutex> lock(singleton_mutex);
            if(!_render_engine)
            {
                this->LoadRenderEngine(_cfg._render_engine_name);
            }
        }
        return *_render_engine;
    }

    bool Context::LowLevelApiValid() const
    {
        return _low_level_api.get() != nullptr;
    }

    bool Context::EngineValid() const
    {
        return _render_engine != nullptr;
    }

    bool Context::isAppValid() const
    {
        return _app != nullptr;
    }
};
