#include <Torch/Torch.hpp>
#include <Torch/Interfaces/Context.hpp>

#include <Torch/Interfaces/Window.hpp>


namespace Torch
{
    void Window::UpdateDpiScale(float scale)
    {

    }

    float Window::_getDPI() const
    {
        return _dpi_scale;
    }
}
