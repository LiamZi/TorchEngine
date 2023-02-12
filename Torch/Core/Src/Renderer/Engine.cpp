#include <Torch/Torch.hpp>

#include <TML/Util.hpp>
#include <glm/glm.hpp>
#include <Torch/Interfaces/Context.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>
#include <Torch/Interfaces/Window.hpp>

#include <mutex>
#include <string>
#include <Torch/Interfaces/Engine.hpp>


namespace Torch
{

    Torch::Engine::Engine()
        : _type{EngineType::ET_NONE}
    {
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
};