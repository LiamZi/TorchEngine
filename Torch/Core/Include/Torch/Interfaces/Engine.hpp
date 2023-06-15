#ifndef __TORCH_CORE_INCLUDE_TORCH_INTERFACES_ENGINE_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_INTERFACES_ENGINE_HPP__

#pragma once

#include <Torch/Interfaces/RenderSettings.hpp>

#include <string_view>
#include <vector>

#include <Torch/Interfaces/LowLevelApi.hpp>
#include <Torch/Renderer/DeviceFeature.hpp>

namespace Torch
{
    FWD_CLASS_SPTR(RasterizerState);
    FWD_CLASS_SPTR(Canvas);

    class TORCH_CORE_API Engine
    {
    public:
        enum class EngineType : unsigned int
        {
            ET_NONE = 0,
            ET_DX,
            ET_OGL,
            ET_ES,
            ET_VK
        };

    private:
        EngineType _type;

    protected:
        std::unique_ptr<LowLevelApi> _low_level_api;
        DeviceFeature _deviceFeature;
        std::string_view _shader_platform_name;
        RasterizerStatePtr _rasterizerState;
        CanvasPtr _canvas[4];

    public:
        virtual ~Engine() noexcept;
        LowLevelApi &LowLevelApiInstance();

    public:
        void CreateRenderWindow(std::string const &name, RenderSettings &settings);
        void DestroyRenderWindow();
        void Resize(uint32_t width, uint32_t height);

        EngineType GetEngineType() const;

    public:
        ReadOnlyProperty<Engine, DeviceFeature> Device_Feature;

    protected:
        Engine();

    private:
        virtual std::wstring &_Name() = 0;
        virtual void _CreateRenderWindow(std::string const& name, RenderSettings const& settings) = 0;
        virtual std::unique_ptr<LowLevelApi> DoCreateLowLevelApi() = 0;
        DeviceFeature& _getDeviceFeature();
    };
};


#endif