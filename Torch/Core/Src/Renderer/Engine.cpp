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
    {
    }

    Engine::~Engine() noexcept = default;

    void Engine::CreateRenderWindow(std::string const &name, RenderSettings &settings)
    {
        this->_CreateRenderWindow(name, settings);
    }

    void Engine::DestroyRenderWindow()
    {
    }
};