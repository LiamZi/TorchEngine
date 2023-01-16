#ifndef __TORCH_CORE_INCLUDE_TORCH_RENDERER_CANVAS_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_RENDERER_CANVAS_HPP__

#include <TML/Util.hpp>


#pragma once

namespace Torch
{
	FWD_CLASS_SPTR(Viewport);

	class TORCH_CORE_API Canvas
	{
	protected:
		uint32_t _left;
		uint32_t _top;
		uint32_t _width;
		uint32_t _height;

		ViewportPtr _viewport;

	public:

	public:
		Canvas();
		virtual ~Canvas() noexcept = default;

		virtual std::wstring const& Description() const = 0;

		void OnError(int errorId, const char* errorDesc);
		void Focus();
		void ApplicationDidEnterBackground();
		void ApplicationDWillEnterForeground();

	public:
		Property<Canvas, ViewportPtr> Viewport;

	private:
		void _setViewport(const ViewportPtr &viewport) ;
		ViewportPtr& _getViewport();

	};
};

#endif