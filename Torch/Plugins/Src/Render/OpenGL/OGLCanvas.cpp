#include <format>
#include <span>
#include <glad_wgl.h>
#include <glad.h>
#include <system_error>

#include <TML/Config.hpp>

#include <TML/Util.hpp>
#include <OpenGL/OGLRenderEngine.hpp>
#include <OpenGL/OGLCanvas.hpp>
#include <Torch/Interfaces/Context.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>
#include <OpenGL/OGLLowLevelApi.hpp>

#include <Torch/Renderer/Viewport.hpp>



namespace Torch
{
	OGLCanvas::OGLCanvas(std::string const& name, RenderSettings const& settings)
		:OGLFrameBuffer(false)
		, FullScreen{this, &OGLCanvas::_getFullScreen, &OGLCanvas::_setFullScreen}
	{

		_name = name;
		_is_full_screen = settings._full_screen;
		_color_bits = NumFormatBits(settings._color_format);

		auto main_wnd = Context::Instance().getApp()->MainWnd();

		_on_exit_size.Add([this](WindowPtr* win, EventState&) -> void
			{
				this->_OnExitSize(*win);
			});

		_on_size.Add([this](WindowPtr* win, EventState& state) -> void
			{
				this->_OnSize(*win, state);
			});


		const float dpi_scale = main_wnd->DPI;
		View_port()->Width = static_cast<uint32_t>(settings._width * dpi_scale + 0.5f);
		View_port()->Height = static_cast<uint32_t>(settings._height * dpi_scale + 0.5f);

		static std::pair<int, int> constexpr all_versions[] =
		{
			std::make_pair(4, 6),
			std::make_pair(4, 5),
			std::make_pair(4, 4),
			std::make_pair(4, 3),
			std::make_pair(4, 2),
			std::make_pair(4, 1)
		};

		std::span<std::pair<int, int> const> available_versions;
		{
			static std::string_view const all_version_names[] =
			{
				"4.6",
				"4.5",
				"4.4",
				"4.3",
				"4.2",
				"4.1",
			};

			static_assert(std::size(all_version_names) == std::size(all_versions));

			uint32_t version_start_index = 0;
			for (size_t i = 0; i < settings.options.size(); ++i)
			{
				std::string_view opt_name = settings.options[i].first;
				std::string_view opt_value = settings.options[i].second;
				if ("version" == opt_name)
				{
					for (uint32_t index = version_start_index; index < std::size(all_version_names); ++index)
					{
						if (all_version_names[i] == opt_value)
						{
							version_start_index = static_cast<uint32_t>(i);
							break;
						}
					}
				}
			}

			available_versions = std::span<std::pair<int, int> const, -1>(all_versions).subspan(version_start_index);
		}
		uint32_t depth_bits = NumDepthBits(settings._depth_stencil_format);
		uint32_t stencil_bits = NumStencilBits(settings._depth_stencil_format);

		_hWnd = main_wnd->HWnd();
		_hDC = ::GetDC(_hWnd);

		uint32_t style;
		if(_is_full_screen)
		{
			View_port()->Left = 0;
			View_port()->Top = 0;

			DEVMODE devMode;
			devMode.dmSize = sizeof(devMode);
			devMode.dmBitsPerPel = _color_bits;
			devMode.dmPelsWidth = View_port()->Width;
			devMode.dmPelsHeight = View_port()->Height;
			devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			::ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

			style = WS_POPUP;
		}
		else
		{
			View_port()->Left = main_wnd->Left;
			View_port()->Top = main_wnd->Top;

			style = WS_OVERLAPPEDWINDOW;
		}

		RECT rect = { 0, 0, static_cast<LONG>(View_port()->Width), static_cast<LONG>(View_port()->Height) };
		::AdjustWindowRect(&rect, style, false);

		::SetWindowLongPtrW(_hWnd, GWL_STYLE, style);
		::SetWindowPos(_hWnd, nullptr, settings._left, settings._top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW | SWP_NOZORDER);

		auto &lowLevelApi = reinterpret_cast<OGLLowLevelApi &>(Context::Instance().Engine().LowLevelApiInstance());

		uint32_t sample_count = settings._sample_count;
		int requested_pixel_format = -1;
		PIXELFORMATDESCRIPTOR requested_pfd{};

		{
			WNDCLASSEXW wc;
			wc.cbSize = sizeof(wc);
			wc.style = CS_OWNDC;
			wc.lpfnWndProc = DefWindowProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = sizeof(this);
			wc.hInstance = ::GetModuleHandle(nullptr);
			wc.hIcon = nullptr;
			wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = L"Canvas";
			wc.hIconSm = nullptr;
			::RegisterClassExW(&wc);

			HWND wnd = ::CreateWindow(wc.lpszClassName, L"", WS_OVERLAPPEDWINDOW, 0, 0 , 1, 1, nullptr, nullptr, wc.hInstance, nullptr);
			HDC dc = ::GetDC(wnd);

			PIXELFORMATDESCRIPTOR pfd;
			memset(&pfd, 0, sizeof(pfd));
			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = static_cast<BYTE>(_color_bits);
			pfd.cDepthBits = static_cast<BYTE>(depth_bits);
			pfd.cStencilBits = static_cast<BYTE>(stencil_bits);
			pfd.iLayerType = PFD_MAIN_PLANE;

			int pixel_format = ::ChoosePixelFormat(dc, &pfd);
			assert(pixel_format != 0);

			::SetPixelFormat(dc, pixel_format, &pfd);
		

			HGLRC rc = lowLevelApi.wglCreateContext(dc);
			lowLevelApi.wglMakeCurrent(dc, rc);

			gladLoadWGL(dc);

			gladLoadGL();

			auto color_format = settings._color_format;
			if(color_format == EF_A2BGR10)
			{
				color_format = EF_ABGR16;
			}

			int r_bits;
			int g_bits;
			int b_bits;
			int a_bits;

			switch (color_format)
			{
			case EF_ARGB8:
			case EF_ABGR8:
				r_bits = 8;
				g_bits = 8;
				b_bits = 8;
				a_bits = 8;
				break;

			case EF_A2BGR10:
				r_bits = 10;
				g_bits = 10;
				b_bits = 10;
				a_bits = 2;
				break;

			case EF_ABGR16F:
				r_bits = 16;
				g_bits = 16;
				b_bits = 16;
				a_bits = 16;
				break;
			default:

				break;
			}

			int pf;
			uint32_t num_formats;
			float float_attrs[] = { 0, 0 };
			BOOL valid;
			do
			{
				std::vector<int> int_attrs =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_RED_BITS_ARB, r_bits,
					WGL_GREEN_BITS_ARB, g_bits,
					WGL_BLUE_BITS_ARB, b_bits,
					WGL_ALPHA_BITS_ARB, a_bits,
					WGL_DEPTH_BITS_ARB, static_cast<int>(depth_bits),
					WGL_STENCIL_BITS_ARB, static_cast<int>(stencil_bits),
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE
				};


				if (IsFloatFormat(color_format))
				{
					int_attrs.push_back(WGL_PIXEL_TYPE_ARB);
					int_attrs.push_back(WGL_TYPE_RGBA_FLOAT_ARB);
				}
				if(sample_count > 1)
				{
					int_attrs.push_back(WGL_SAMPLE_BUFFERS_ARB);
					int_attrs.push_back(GL_TRUE);
					int_attrs.push_back(WGL_SAMPLES_ARB);
					int_attrs.push_back(static_cast<int>(sample_count));
				}
				if(settings._stereo_method == STM_LCDShutter)
				{
					int_attrs.push_back(WGL_STEREO_ARB);
					int_attrs.push_back(GL_TRUE);
				}
				int_attrs.push_back(0);
				int_attrs.push_back(0);

				valid = wglChoosePixelFormatARB(dc, &int_attrs[0], float_attrs, 1, &pf, &num_formats);
				//valid = wglGetPixelFormatAttribivARB(dc, )
				
				if(valid && (num_formats > 0))
				{
					break;
				}
				else
				{
					--sample_count;
				}

			} while (sample_count > 0);

			if(valid && (sample_count > 0))
			{
				requested_pixel_format = pf;
				requested_pfd = pfd;

				lowLevelApi.wglMakeCurrent(dc, nullptr);
				lowLevelApi.wglDeleteContext(rc);
				::ReleaseDC(wnd, dc);
				::DestroyWindow(wnd);
			}
		}

		::SetPixelFormat(_hDC, requested_pixel_format, &requested_pfd);
		_hRC = lowLevelApi.wglCreateContext(_hDC);
		lowLevelApi.wglMakeCurrent(_hDC, _hRC);

		if(GLAD_WGL_ARB_create_context)
		{
			int flags = 0;
#ifdef TORCH_DEBUG
			bool const debug_context = true;
#endif

			if(debug_context)
			{
				flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
			}

			int attrs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 0,
				WGL_CONTEXT_MINOR_VERSION_ARB, 0,
				WGL_CONTEXT_FLAGS_ARB, flags,
				WGL_CONTEXT_PROFILE_MASK_ARB,
				WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
			};

			for(size_t i = 0; i < available_versions.size(); ++i)
			{
				attrs[1] = available_versions[i].first;
				attrs[3] = available_versions[i].second;
				HGLRC hrc_new = wglCreateContextAttribsARB(_hDC, nullptr, attrs);
				if(hrc_new != nullptr)
				{
					lowLevelApi.wglMakeCurrent(_hDC, nullptr);
					lowLevelApi.wglDeleteContext(_hRC);
					_hRC = hrc_new;

					lowLevelApi.wglMakeCurrent(_hDC, _hRC);

					gladLoadGL();
					break;
				}
			}
		}

		if(!GLAD_GL_VERSION_4_1)
		{
			throw std::system_error(std::make_error_code(std::errc::function_not_supported), std::format("{}: {}", std::move(__FILE__), __LINE__));
		}

		glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
		glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
		glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

#if defined TORCH_PLATFORM_WINDOWS
		if(GLAD_WGL_EXT_swap_control)
		{
			wglSwapIntervalEXT(settings._sync_interval);
		}
#endif

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//View_port()->Left = 0;
		//View_port()->Top = 0;
		

		std::wstring vendor;
		std::wstring renderer;
		std::wstring version;

		Convert(vendor, reinterpret_cast<char const*>(glGetString(GL_VENDOR)));
		Convert(renderer, reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
		Convert(version, reinterpret_cast<char const*>(glGetString(GL_VERSION)));

		_description = vendor + L" " + renderer + L" " + version;
		if(sample_count > 1)
		{
			_description += L" (" + std::to_wstring(sample_count) + L"x AA)";
		}
	}

    OGLCanvas::~OGLCanvas()
	{
		this->Destroy();
	}

	void OGLCanvas::Destroy()
	{
#if defined TORCH_PLATFORM_WINDOWS
		if(_hWnd != nullptr)
		{
			if(_hDC != nullptr)
			{
				auto &low_level_api = reinterpret_cast<OGLLowLevelApi &>(Context::Instance().Engine().LowLevelApiInstance());
				low_level_api.wglMakeCurrent(_hDC, nullptr);
				if(_hRC != nullptr)
				{
					low_level_api.wglDeleteContext(_hRC);
					_hRC = nullptr;
				}
				::ReleaseDC(_hWnd, _hDC);
				_hDC = nullptr;
			}

			if(_is_full_screen)
			{
				::ChangeDisplaySettings(nullptr, 0);
				ShowCursor(TRUE);
			}
		}
#endif
	}

	void OGLCanvas::Present()
	{
#if defined TORCH_PLATFORM_WINDOWS
		::SwapBuffers(_hDC);
#endif
	}

	std::wstring const& OGLCanvas::Description() const
	{
		return _description;
	}

    void OGLCanvas::Resize(uint32_t width, uint32_t height)
    {
		_width = width;
		_height = height;

		_viewport->Width = width;
		_viewport->Height = height;
    }

    void OGLCanvas::Reposition(uint32_t left, uint32_t top)
    {
		_left = left;
		_top = top;
    }

    void OGLCanvas::WindowMovedOrResized(const WindowPtr  &win)
    {
		float const dpi_scale = win->DPI;

#if defined TORCH_PLATFORM_WINDOWS
		::RECT rect;
		::GetClientRect(_hWnd, &rect);

		uint32_t new_left = rect.left;
		uint32_t new_top = rect.top;

		if((new_left != _left) || (new_top != _top))
		{
			Reposition(new_left, new_top);
		}

		uint32_t new_width = rect.right - rect.left;
		uint32_t new_height = rect.bottom - rect.top;
#endif

		if((new_width != _width) || (new_height != _height))
		{
			auto width = static_cast<uint32_t>(new_width / dpi_scale + 0.5f);
			auto height = static_cast<uint32_t>(new_height / dpi_scale + 0.5);
			Context::Instance().Engine().Resize(width, height);
		}
    }


    void OGLCanvas::_OnExitSize(WindowPtr const &win)
    {
		this->WindowMovedOrResized(win);
	}

	void OGLCanvas::_OnSize(WindowPtr const& win, EventState &state)
	{
		if(state._target)
		{
			if(win->Ready)
			{
				WindowMovedOrResized(win);
			}
		}
	}

	void OGLCanvas::_setFullScreen(const bool full)
    {
		if(_is_full_screen != full)
		{
			View_port()->Left = 0;
			View_port()->Top = 0;

#if defined TORCH_PLATFORM_WINDOWS
			uint32_t style;
			if(full)
			{
				DEVMODE devMode;
				devMode.dmSize = sizeof(devMode);
				devMode.dmBitsPerPel = _color_bits;
				devMode.dmPelsWidth = View_port()->Width;
				devMode.dmPelsHeight = View_port()->Height;
				devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				::ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
				style = WS_POPUP;
			}
			else
			{
				::ChangeDisplaySettings(nullptr, 0);

				style = WS_OVERLAPPEDWINDOW;
			}

			::SetWindowLongPtrW(_hWnd, GWL_STYLE, style);

			RECT rect = {0, 0, static_cast<LONG>(View_port()->Width), static_cast<LONG>(View_port()->Height)};
			::AdjustWindowRect(&rect, style, false);
			View_port()->Width = rect.right - rect.left;
			View_port()->Height = rect.bottom - rect.top;

			_is_full_screen = full;

			::ShowWindow(_hWnd, SW_SHOWNORMAL);
			::UpdateWindow(_hWnd);
#endif
		}
    }

    bool OGLCanvas::_getFullScreen() const
    {
        return _is_full_screen;
    }
};
