#include <Torch/Torch.hpp>

#include <TML/Util.hpp>
#include <glm/glm.hpp>
#include <Torch/Interfaces/Context.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>
#include <Torch/Interfaces/Window.hpp>

#include <mutex>
#include <string>
#include <Torch/Interfaces/Engine.hpp>
#include "Engine.hpp"


namespace Torch
{

    Engine::Engine()
        : _type{EngineType::ET_NONE}
		, Device_Feature{this, &Engine::_getDeviceFeature}
    {
    }

    DeviceFeature& Engine::_getDeviceFeature()
    {
        return _deviceFeature;
    }

    Engine::EngineType Engine::GetEngineType() const
    {
        return _type;
    }

    Engine::~Engine() noexcept = default;

    LowLevelApi &Engine::LowLevelApiInstance()
    {
        if(!_low_level_api)
        {
            _low_level_api = this->DoCreateLowLevelApi();
        }

        return *_low_level_api;
    }

    void Engine::CreateRenderWindow(std::string const &name, RenderSettings &settings)
    {
        this->_CreateRenderWindow(name, settings);
    }

    void Engine::DestroyRenderWindow()
    {

    }

    void Torch::Engine::Resize(uint32_t width, uint32_t height)
    {
        uint32_t const old_screen_width = _defau
    }
};