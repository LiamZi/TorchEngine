﻿#ifndef __TORCH_CORE_INCLUDE_TORCH_INTERFACES_ENGINE_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_INTERFACES_ENGINE_HPP__

#pragma once

#include <Torch/Interfaces/RenderSettings.hpp>

#include <string_view>
#include <vector>


namespace Torch
{
    class TORCH_CORE_API Engine
    {
    

    public:
        Engine();
        virtual ~Engine() noexcept;

    public:
        void CreateRenderWindow(std::string const &name, RenderSettings &settings);
        void DestroyRenderWindow();

    private:
        virtual std::wstring &_Name() = 0;
        virtual void _CreateRenderWindow(std::string const& name, RenderSettings const& settings) = 0;

    };
};


#endif