
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
		Window* win = reinterpret_cast<Window*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
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
        return TRUE;
    }
#endif

    Window::Window(std::string const &name, void* native_wnd)
    {
        
    }

    LRESULT Window::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return NULL;
    }
};