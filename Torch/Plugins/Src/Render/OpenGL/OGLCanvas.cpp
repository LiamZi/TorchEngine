#include <span>
#include <TML/Util.hpp>
#include <OpenGL/OGLRenderEngine.hpp>
#include <OpenGL/OGLCanvas.hpp>
#include <Torch/Interfaces/Context.hpp>

#include "Torch/Interfaces/App3DFramework.hpp"

namespace Torch
{
	OGLCanvas::OGLCanvas(std::string const& name, RenderSettings const& settings)
		:OGLFrameBuffer(false)
		, FullScreen{this, &OGLCanvas::_getFullScreen, &OGLCanvas::_setFullScreen}
	{
		_name = name;
		_is_full_screen = settings._full_screen;
		_color_bits = 32;

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
		_width = static_cast<uint32_t>(settings._width * dpi_scale + 0.5f);
		_height = static_cast<uint32_t>(settings._height * dpi_scale + 0.5f);

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
			for(size_t i = 0; i < settings.options.size(); ++i)
			{
				std::string_view opt_name = settings.options[i].first;
				std::string_view opt_value = settings.options[i].second;
				if("version" == opt_name)
				{
					for(uint32_t index = version_start_index; index < std::size(all_version_names); ++index)
					{
						if(all_version_names[i] == opt_value)
						{
							version_start_index = i;
							break;
						}
					}
				}
			}

			available_versions = std::span<std::pair<int, int> const, -1>(all_versions).subspan(version_start_index);

			uint32_t depth_bits = 24;
			uint32_t stencil_bits = 8;

			_hWnd = main_wnd->HWnd();
			_hDC = ::GetDC(_hWnd);

			uint32_t style;
			if(_is_full_screen)
			{
				_left = 0;
				_top = 0;

				DEVMODE devMode;
				devMode.dmSize = sizeof(devMode);
				devMode.dmBitsPerPel = _color_bits;
				devMode.dmPelsWidth = _width;
				devMode.dmPelsHeight = _height;
				devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				::ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

				style = WS_POPUP;
			}
			else
			{
				_left = main_wnd->Left;
				_top = main_wnd->Top;

				style = WS_OVERLAPPEDWINDOW;
			}

			RECT rect = { 0, 0, static_cast<LONG>(_width), static_cast<LONG>(_height) };
			::AdjustWindowRect(&rect, style, false);

			::SetWindowLongPtrW(_hWnd, GWL_STYLE, style);
			::SetWindowPos(_hWnd, nullptr, settings._left, settings._top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW | SWP_NOZORDER);

			auto &render_engine = reinterpret_cast<OGLRenderEngine &>(Context::Instance().)


		}


		_description = L"OGL Canvas.";
	}

    OGLCanvas::~OGLCanvas() = default;

	void OGLCanvas::Destroy()
	{
	}

	void OGLCanvas::Present()
	{
	}

	std::wstring const& OGLCanvas::Description() const
	{
		return _description;
	}

    void OGLCanvas::Resize(uint32_t width, uint32_t height)
    {
    }

	void OGLCanvas::_OnExitSize(WindowPtr const& win)
	{
	}

	void OGLCanvas::_OnSize(WindowPtr const& win, EventState &state)
	{
	}

	void OGLCanvas::_setFullScreen(const bool full)
    {
		if(_is_full_screen != full)
		{
			_left = 0;
			_top = 0;

#if defined TORCH_PLATFORM_WINDOWS
			uint32_t style;
			if(full)
			{
				DEVMODE devMode;
				devMode.dmSize = sizeof(devMode);
				devMode.dmBitsPerPel = _color_bits;
				devMode.dmPelsWidth =  _width;
				devMode.dmPelsHeight = _height;
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

			RECT rect = {0, 0, static_cast<LONG>(_width), static_cast<LONG>(_height)};
			::AdjustWindowRect(&rect, style, false);
			_width = rect.right - rect.left;
			_height = rect.bottom - rect.top;

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
