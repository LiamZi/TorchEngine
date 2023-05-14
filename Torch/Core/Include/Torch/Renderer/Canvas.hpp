#ifndef __TORCH_CORE_INCLUDE_TORCH_RENDERER_CANVAS_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_RENDERER_CANVAS_HPP__

#include <TML/Util.hpp>


#pragma once

namespace Torch
{
	class TORCH_CORE_API Canvas
	{
	public:
		enum class BindType
		{
			Color0 = 0,
			Color1,
			Color2,
			Color3,
			Color4,
			Color5,
			Color6,
			Color7
		};

	protected:
		uint32_t _left;
		uint32_t _top;
		uint32_t _width;
		uint32_t _height;


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

		void Bind();
	};
};

#endif