#ifndef __TORCH_ENGINE_CORE_RENDERER_RENDER_SETTINGS_HPP__
#define __TORCH_ENGINE_CORE_RENDERER_RENDER_SETTINGS_HPP__

#pragma once

#include <vector>

namespace Torch
{
	class RenderSettings
	{
	public:
		bool _hide_win;
		bool _full_screen;
		int32_t _left;
		int32_t _top;
		uint32_t _width;
		uint32_t _height;
		bool _keep_on_screen;
		uint32_t _sample_count;
		uint32_t _sample_quality;
		uint32_t _sync_interval;
		bool _hdr;
		bool _fft_lens_effects;
		bool _ppaa;
		bool _gamma;
		bool _color_gradient;
		float _max_dpi_scale;
		bool _debug_context;

		RenderSettings()
			: _hide_win(false)
			, _full_screen(false)
			, _left(0)
			, _top(0)
			, _sample_count{1}
			, _sample_quality{0}
			, _hdr{false}
			, _fft_lens_effects{false}
			, _ppaa{false}
			, _gamma{false}
			, _color_gradient{false}
			, _max_dpi_scale{2.0f}
			, _debug_context{true}
			, _width{1270}
			, _height{768}
		{
			
		}
	};
};

#endif
