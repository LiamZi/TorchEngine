#ifndef __TORCH_CORE_INCLUDE_TORCH_CONTEXT_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_CONTEXT_HPP__

#pragma once

#include <string>
#include <memory>
#include <assert.h>

#include <TML/Util.hpp>
#include <TML/DllLoader.hpp>
#include <Torch/Interfaces/RenderSettings.hpp>


namespace Torch
{
    class App3DFramework;
    class ThreadPool;
    class Engine;

    struct ContextCfg
    {
        std::string _render_factory_name;
        std::string _input_factory_name;
        std::string _scene_manager_name;

        RenderSettings _graphics_cfg;
        bool _deferred_rendering;
        bool _perf_profiler;
        bool _location_sensor;
    };

    class TORCH_CORE_API Context
    {
    private:
        static std::unique_ptr<Context> _context_instance;
        App3DFramework *_app;
        std::unique_ptr<ThreadPool> _tp_instance;
        ContextCfg _cfg;

    private:
        DllLoader _render_loader;

        std::unique_ptr<Engine> _render_engine;

    public:
        Context();
        ~Context();

    public:
        static Context &Instance();
        static void Destroy();
        void Suspend();
        void Resume();
        void LoadRenderEngine(std::string const& name);

    public:
        ReadOnlyProperty<Context, bool> AppValid;
        
        ThreadPool &ThreadPools();


        void setApp(App3DFramework *app);
        App3DFramework *getApp();
        void setConfig(const ContextCfg &cfg);
        ContextCfg &getConfig();
        
        
    private:
        void DestoryAll();
        bool isAppValid() const;
      

    };
};

#endif