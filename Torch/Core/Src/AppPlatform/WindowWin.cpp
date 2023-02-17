
#include <Torch/Torch.hpp>
#ifdef TORCH_PLATFORM_WINDOWS_DESKTOP
#include <TML/Util.hpp>
#include <Torch/Interfaces/Window.hpp>

#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
#include <VersionHelpers.h>
#include <ShellScalingAPI.h>
#endif
#endif

#include <windowsx.h>
//#include <algorithm>
//#include <utility>

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
        (void *)(dc_mon);
        (void *)(rc_mon);
        DllLoader shcore;
		if (shcore.Load("SHCore.dll"))
		{
			typedef HRESULT (CALLBACK *GetDpiForMonitorFunc)(HMONITOR mon, MONITOR_DPI_TYPE dpi_type, UINT* dpi_x, UINT* dpi_y);
			auto* DynamicGetDpiForMonitor = reinterpret_cast<GetDpiForMonitorFunc>(shcore.GetProcAddress("GetDpiForMonitor"));
			if (DynamicGetDpiForMonitor)
			{
				UINT dpi_x, dpi_y;
				if (S_OK == DynamicGetDpiForMonitor(mon, MDT_DEFAULT, &dpi_x, &dpi_y))
				{
					Window* win = reinterpret_cast<Window *>(lparam);
					win->UpdateDpiScale(std::max(win->_dpi_scale, static_cast<float>(std::max(dpi_x, dpi_y)) / USER_DEFAULT_SCREEN_DPI));
				}
			}

			shcore.Free();
		}

		return TRUE;
    }
#endif

    Window::Window(std::string const &name, RenderSettings const &setting, void* native_wnd)
    : _active{false}
    , _ready{false}
    , _closed{false}
    , _keep_Screen_on{true}
    , _dpi_scale{1}
    , _effective_dpi_scale{1}
    , _win_Roation{WindowRotation::WR_Identity}
    , _hide{setting._hide_win}
    , DPI{this, &Window::_getDPI}
    , Active(this, &Window::isActive, &Window::setActive)
    , Ready(this, &Window::isReady, &Window::setReady)
    , Left(this, &Window::_getLeft)
    , Top(this, &Window::_getTop)
    , Width(this, &Window::_getWidth)
    , Height(this, &Window::_getHeight)
    {
        DetectsDpi();
        KeepScreenOn();

        Convert(_wname, name);

        if(native_wnd != nullptr)
        {
            _wnd = static_cast<HWND>(native_wnd);
            _default_wnd_proc = reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(_wnd, GWLP_WNDPROC));
            _win_style = static_cast<uint32_t>(GetWindowLongPtrW(_wnd, GWL_STYLE));
            ::SetWindowLongPtrW(_wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
            _external_wnd = true;
        }
        else
        {
            HINSTANCE hInst = ::GetModuleHandle(nullptr);

            //注册 window class
            WNDCLASSEXW wc;
            wc.cbSize = sizeof(wc);
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = WndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = sizeof(this);
            wc.hInstance = hInst;
            wc.hIcon = nullptr;
            wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
            wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
            wc.lpszMenuName = nullptr;
            wc.lpszClassName = _wname.c_str();
            wc.hIconSm = nullptr;
            ::RegisterClassExW(&wc);

            //render setting full screen 
            if(0)
            {
                _win_style = WS_POPUP;
            }
            else
            {
                _win_style = WS_OVERLAPPEDWINDOW;
            }

            RECT rect = { 0, 0, static_cast<LONG>(setting._width * _dpi_scale + 0.5f), static_cast<LONG>(setting._height * _dpi_scale + 0.5f)};
            ::AdjustWindowRect(&rect, _win_style, false);

            _wnd = ::CreateWindowW(_wname.c_str(), _wname.c_str(), _win_style, 0, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInst, nullptr);

            _default_wnd_proc = ::DefWindowProc;
            _external_wnd = false;
        }

        RECT rect;
        ::GetClientRect(_wnd, &rect);
        _left = rect.left;
        _top = rect.top;
        _width = rect.right - rect.left;
        _height = rect.bottom - rect.top;

        ::SetWindowLongPtrW(_wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        ::ShowWindow(_wnd, _hide ? SW_HIDE : SW_SHOWNORMAL);
        ::UpdateWindow(_wnd);

        _ready = true;
    }

    Window::~Window()
    {
        if(_keep_Screen_on)
        {
#if defined(TORCH_PLATFROM_WINDOWS_DESKTOP)
            ::SetThreadExecutionState(ES_CONTINUOUS);
#endif
        }

        if(_wnd != nullptr)
        {
            ::SetWindowLongPtrW(_wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
            if(!_external_wnd)
            {
                ::DestroyWindow(_wnd);
            }

            _wnd = nullptr;
        }
    }

    LRESULT Window::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_ACTIVATE:
            _active = (WM_ACTIVATE != LOWORD(wParam));
            // this->onActive()(*this, _active);
            break;

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
            break;

        case WM_ENTERSIZEMOVE:
            _ready = false;
            break;

        case WM_EXITSIZEMOVE:
            _ready = true;
            break;

        case WM_SIZE:
        {
            RECT rect;
            ::GetClientRect(_wnd, &rect);
            _left = rect.left;
            _top = rect.top;
            _width = rect.right - rect.left;
            _height = rect.bottom - rect.top;

            if((SIZE_MAXHIDE == wParam) || (SIZE_MINIMIZED == wParam))
            {
                _active = false;
                // this->onSize();
            }
            else
            {
                _active = true;
                // this->onSize();
            }
        }
            break;
        
        case WM_GETMINMAXINFO:
            reinterpret_cast<MINMAXINFO *>(lParam)->ptMinTrackSize.x = 100;
            reinterpret_cast<MINMAXINFO *>(lParam)->ptMinTrackSize.y = 100;
            break;
        
        case WM_SETCURSOR:
            // this->onSetCursor();
            break;

        case WM_CHAR:
            break;

        case WM_INPUT:
            break;

#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
        case WM_POINTERDOWN:
        {
            POINT p = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ::ScreenToClient(this->HWnd(), &p);
            // this->onPointerDown();
        }
            break;
        
        case WM_POINTERUP:
        {
            POINT p = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ::ScreenToClient(this->HWnd(), &p);
            // this->onPointerUp();
        }

            break;
        
        case WM_POINTERUPDATE:
        {
            POINT p = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ::ScreenToClient(this->HWnd(), &p);
            // this->onPointerUpdate();
            // IS_POINTER_INCONTACT_WPARAM(wParam);
        }

            break;

        case WM_POINTERWHEEL:
        {
            POINT p = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ::ScreenToClient(this->HWnd(), &p);
            // this->onPointerWheel();
            // GET_WHEEL_DELTA_WPARAM(wParam);
        }

            break;

        case WM_DPICHANGED:
        {
            RECT rect;
            ::GetClientRect(_wnd, &rect);
            rect.left = static_cast<int32_t>(rect.left / _dpi_scale + 0.5f);
            rect.top = static_cast<int32_t>(rect.top/ _dpi_scale + 0.5f);
            rect.right = static_cast<uint32_t>(rect.right / _dpi_scale + 0.5f);
            rect.bottom = static_cast<uint32_t>(rect.bottom / _dpi_scale + 0.5f);

            this->UpdateDpiScale(static_cast<float>(HIWORD(wParam)) / USER_DEFAULT_SCREEN_DPI);

            rect.left = static_cast<int32_t>(rect.left * _dpi_scale + 0.5f);
            rect.top = static_cast<int32_t>(rect.top * _dpi_scale + 0.5f);
            rect.right = static_cast<uint32_t>(rect.right * _dpi_scale + 0.5f);
            rect.bottom = static_cast<uint32_t>(rect.bottom * _dpi_scale + 0.5f);

            ::AdjustWindowRect(&rect, _win_style, false);

            ::SetWindowPos(this->HWnd(), HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOREPOSITION);

        }
            break;
#endif
        case WM_CLOSE:
            // this->Close();
            _active = false;
            _ready = false;
            _closed = true;
            ::PostQuitMessage(0);
            return 0;

        case WM_SYSCOMMAND:
            if(_keep_Screen_on)
            {
                switch (wParam)
                {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                    return 0;
                
                default:
                    break;
                }
            }
            break;
        }

        return _default_wnd_proc(hWnd, uMsg, wParam, lParam);
    }

    void Window::DetectsDpi()
    {
#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
        DllLoader shcore;
        if(shcore.Load("SHCore.dll"))
        {
            typedef HRESULT (WINAPI *SetProcessDpiAwarenessFunc)(PROCESS_DPI_AWARENESS value);
            auto *DynamicSetProcessDpiAwareness = reinterpret_cast<SetProcessDpiAwarenessFunc>(shcore.GetProcAddress("SetProcessDpiAwareness"));

            DynamicSetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

            shcore.Free();    
        }

        typedef NTSTATUS (WINAPI *RtlGetVersionFunc)(OSVERSIONINFOEXW *pVersionInfomation);
        DllLoader ntdll;
        ntdll.Load("ntdll.dll");
        auto *DynamicRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(ntdll.GetProcAddress("RtlGetVersion"));
        if(DynamicRtlGetVersion)
        {
            OSVERSIONINFOEXW os_ver_info;
            os_ver_info.dwOSVersionInfoSize = sizeof(os_ver_info);
            DynamicRtlGetVersion(&os_ver_info);

            if((os_ver_info.dwMajorVersion > 6) || ((6 == os_ver_info.dwMajorVersion) && (os_ver_info.dwMinorVersion >= 3)))
            {
                HDC desktop_dc = ::GetDC(nullptr);
                ::EnumDisplayMonitors(desktop_dc, nullptr, EnumMonProc, reinterpret_cast<LPARAM>(this));
                ::ReleaseDC(nullptr, desktop_dc);
            }
        }
#endif
    }

    void Window::KeepScreenOn()
    {
        if(_keep_Screen_on)
        {
#if defined(TORCH_PLATFORM_WINDOWS_DESKTOP)
            ::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
#endif     
        }
    }
};