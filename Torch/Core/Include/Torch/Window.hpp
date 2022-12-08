#ifndef __TORCH_CORE_INCLUDE_TORCH_WINDOW_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_WINDOW_HPP__

#pragma once

#include <Torch/Torch.hpp>

#if defined TORCH_PLATFORM_WINDOWS_DESKTOP
#include <Windows.h>
#endif

#include <string>

namespace Torch
{
    class TORCH_CORE_API Window
    {


    public:
        enum class WindowRotation : int
        {
            WR_Unspecified,
			WR_Identity,
			WR_Rotate90,
			WR_Rotate180,
			WR_Rotate270
        };

    private:
        int32_t _left;
        int32_t _top;
        uint32_t _width;
        uint32_t _height;

        bool _active;
        bool _ready;
        bool _closed;
        float _dpi_scale;
        float _effective_dpi_scale;
        WindowRotation _win_Roation;

        bool _hide;
        bool _external_wnd;
        std::wstring _wname;

    private:
#if defined TORCH_PLATFORM_WINDOWS_DESKTOP
        HWND _wnd;
        WNDPROC _default_wnd_proc;
        uint32_t _win_style;

#endif

    public:
        Window(std::string const &name, void* native_wnd);
        ~Window();

    private:
        void UpdateDpiScale(float scale);
    
#if defined TORCH_PLATFORM_WINDOWS
#if defined TORCH_PLATFORM_WINDOWS_DESKTOP
        HWND HWnd() const
        {
            return _wnd;
        }

    private:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
		static BOOL CALLBACK EnumMonProc(HMONITOR mon, HDC dc_mon, RECT* rc_mon, LPARAM lparam) noexcept;
#endif
        void KeepScreenOn();
        LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
#else
        void DetectsOrientation();
#endif
        void DetectsDpi();

    };
};

#endif