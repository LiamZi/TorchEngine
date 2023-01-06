#ifndef __TORCH_CORE_INCLUDE_TORCH_CONTEXT_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_CONTEXT_HPP__

#pragma once

#include <string>
#include <memory>
#include <assert.h>

#include <TML/DllLoader.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>

namespace Torch
{

    struct ContextCfg
    {

    };

    class TORCH_CORE_API Context
    {
    private:
        static std::unique_ptr<Context> _context_instance;

        App3DFramework *_app;
        
    public:
        Context();
        ~Context();

    public:
        static Context &Instance();
        static void Destroy();
        void Suspend();
        void Resume();
        

    private:
        void DestoryAll();

    };
};

#endif