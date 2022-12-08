
#include <Torch/Torch.hpp>
#ifdef TORCH_PLATFORM_WINDOWS_DESKTOP
#include <TML/Util.hpp>
#include <Torch/Window.hpp>

#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
#include <VersionHelpers.h>
#include <ShellScalingAPI.h>
#endif
#endif

#include <windowsx.h>

namespace Torch
{
   LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
   {
        Window *win = reinterpret_cast<Window *>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        if (win != nullptr)
        {
            return win->MsgProc(hWnd, uMsg, wParam, lParam);
        }
        else
        {
            return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
   }  

#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
    BOOL Window::EnumMonProc(HMONITOR mon, HDC dc_mon, RECT *rc_mon, LPARAM lparam) noexcept
    {
        
    }
#endif

    Window::Window(std::string const& name, void* native_wnd)
        : _active{false}
        , _ready{false}
        , _closed{false}
        , _dpi_scale{1}
        , _effective_dpi_scale{1}
        , _win_Roation{WindowRotation::WR_Identity}
        , _hide{false}
    {

    }

    Window::~Window()
    {

    }
};