#ifndef __TORCH_CORE_INCLUDE_TORCH_WINDOW_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_WINDOW_HPP__

#pragma once

#include <Torch/Torch.hpp>
#include <Torch/Interfaces/RenderSettings.hpp>

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
        bool _keep_Screen_on;
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
        Window(std::string const &name, RenderSettings const &setting, void* native_wnd);
        ~Window();

        HWND HWnd() const
        {
            return _wnd;
        }

    public:
        ReadOnlyProperty<Window, float> DPI;
        Property<Window, bool> Active;
        Property<Window, bool> Ready;
        ReadOnlyProperty<Window, uint32_t> Left;
        ReadOnlyProperty<Window, uint32_t> Top;
        ReadOnlyProperty<Window, uint32_t> Width;
        ReadOnlyProperty<Window, uint32_t> Height;

    private:
        void UpdateDpiScale(float scale);
        float _getDPI() const;
        bool isActive() const
        {
            return _active;
        }

        void setActive(const bool active)
        {
            _active = active;
        }

        bool isReady() const
        {
            return _ready;
        }

        void setReady(bool ready)
        {
            _ready = ready;
        }

        uint32_t _getLeft() const
        {
            return _left;
        }

        uint32_t _getTop() const
        {
            return _top;
        }

        uint32_t _getWidth() const
        {
            return _width;
        }

        uint32_t _getHeight() const
        {
            return _height;
        }

#if defined TORCH_PLATFORM_WINDOWS
#if defined TORCH_PLATFORM_WINDOWS_DESKTOP


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