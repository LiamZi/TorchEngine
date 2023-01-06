#ifndef __TORCH_CORE_INCLUDE_TORCH_CONTEXT_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_CONTEXT_HPP__

#pragma once

#include <string>
#include <memory>
#include <assert.h>

#include <TML/Util.hpp>
#include <TML/DllLoader.hpp>


namespace Torch
{
    FWD_CLASS_SPTR(App3DFramework);

    struct ContextCfg
    {

    };

    class TORCH_CORE_API Context
    {
    private:
        static std::unique_ptr<Context> _context_instance;
        App3DFrameworkPtr _app;

    public:
        Context();
        ~Context();

    public:
        static Context &Instance();
        static void Destroy();
        void Suspend();
        void Resume();

    public:
        Property<Context, App3DFrameworkPtr> App;
        ReadOnlyProperty<Context, bool> AppValid;
        
    private:
        void DestoryAll();
        void setApp(const App3DFrameworkPtr &app);
        App3DFrameworkPtr &getApp();
        bool isAppValid() const;

    };
};

#endif