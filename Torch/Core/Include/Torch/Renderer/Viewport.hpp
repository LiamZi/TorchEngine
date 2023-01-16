#ifndef __TORCH_CORE_INCLUDE_TORCH_RENDERER_VIEWPORT_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_RENDERER_VIEWPORT_HPP__

#pragma once

#include <TML/Util.hpp>

namespace Torch
{
	class TORCH_CORE_API Viewport
	{
	private:
		int _left;
		int _top;
		int _width;
		int _height;

	public:
		Viewport();
		Viewport(int left, int top, int width, int height);
		~Viewport() = default;

	public:
		Property<Viewport, int> Left;
		Property<Viewport, int> Top;
		Property<Viewport, int> Width;
		Property<Viewport, int> Height;

	private:
		void _setLeft(const int left);
		int _getLeft() const;
		void _setTop(const int top);
		int _getTop() const;
		void _setWidth(const int width);
		int _getWidth() const;
		void _setHeight(const int height);
		int _getHeight() const;
	};
};

#endif
