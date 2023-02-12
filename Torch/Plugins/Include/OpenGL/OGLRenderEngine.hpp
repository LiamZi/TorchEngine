#ifndef __TORCH_PLUGINS_OGL_RENDER_ENGINE_HPP__
#define __TORCH_PLUGINS_OGL_RENDER_ENGINE_HPP__

#pragma once

#include <glm/vec2.hpp>
#include <vector>
#include <map>
#include <set>

#include <Torch/Interfaces/Engine.hpp>

namespace Torch
{
    class OGLRenderEngine final : public Engine
    {
    public:
        OGLRenderEngine();
        ~OGLRenderEngine() override;

    public:
        ReadOnlyProperty<OGLRenderEngine, std::wstring &> Name;

    private:
        virtual std::wstring &_Name() override;
        virtual void _CreateRenderWindow(std::string const& name, RenderSettings const& settings) override;
        std::unique_ptr<LowLevelApi> DoCreateLowLevelApi() override;
    };
};

#endif