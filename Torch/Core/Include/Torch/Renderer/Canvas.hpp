#ifndef __TORCH_CORE_INCLUDE_TORCH_RENDERER_CANVAS_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_RENDERER_CANVAS_HPP__

#include <TML/Util.hpp>


#pragma once

namespace Torch
{
	class TORCH_CORE_API Canvas
	{
	public:

	public:
		Canvas();
		virtual ~Canvas() noexcept = default;

		virtual std::wstring const& Description() const = 0;

		void OnError(int errorId, const char* errorDesc);
		void Focus();
		void ApplicationDidEnterBackground();
		void ApplicationDWillEnterForeground();

		virtual void Present();
		virtual void WaitOnPresent();
	};
};

#endif