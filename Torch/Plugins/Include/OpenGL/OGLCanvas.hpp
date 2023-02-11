#ifndef __TORCH_PLUGINS_OGL_CANVAS_HPP__
#define __TORCH_PLUGINS_OGL_CANVAS_HPP__


#pragma once

#include <Torch/Interfaces/RenderSettings.hpp>
#include <OpenGL/OGLFrameBuffer.hpp>
#include <TML/Observable.hpp>

namespace Torch
{
	FWD_CLASS_SPTR(Window);

	class OGLCanvas final : public OGLFrameBuffer
	{
	private:
		std::string _name;

		HWND _hWnd;
		HGLRC _hRC;
		HDC _hDC;

		bool _is_full_screen;
		uint32_t _color_bits;
		std::wstring _description;

		Observable<WindowPtr> _on_size;
		Observable<WindowPtr> _on_exit_size;


	public:
		OGLCanvas(std::string const &name, RenderSettings const &settings);
		virtual ~OGLCanvas() override;

		void Destroy();
		void Present() override;

		std::wstring const &Description() const override;

		void Resize(uint32_t width, uint32_t height);
		
	public:
		Property<OGLCanvas, bool> FullScreen;

	private:
		void _OnExitSize(WindowPtr const& win);
		void _OnSize(WindowPtr const& win, EventState &state);


	private:
		void _setFullScreen(const bool full);
		bool _getFullScreen() const;
	};
};

#endif	
