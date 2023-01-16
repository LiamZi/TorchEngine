#include <Torch/Torch.hpp>
#include <Torch/Renderer/Viewport.hpp>

namespace Torch
{
	Viewport::Viewport()
		: _left(0)
		, _top(0)
		, _width(0)
		, _height(0)
		, Left(this, &Viewport::_getLeft, &Viewport::_setLeft)
		, Top(this, &Viewport::_getTop, &Viewport::_setTop)
		, Width(this, &Viewport::_getWidth, &Viewport::_setWidth)
		, Height(this, &Viewport::_getHeight, &Viewport::_setHeight)
	{
	}

	Viewport::Viewport(int left, int top, int width, int height)
		: _left(left)
		, _top(top)
		, _width(width)
		, _height(height)
		, Left(this, &Viewport::_getLeft, &Viewport::_setLeft)
		, Top(this, &Viewport::_getTop, &Viewport::_setTop)
		, Width(this, &Viewport::_getWidth, &Viewport::_setWidth)
		, Height(this, &Viewport::_getHeight, &Viewport::_setHeight)
	{
	}

	void Viewport::_setLeft(const int left)
	{
		_left = left;
	}

	int Viewport::_getLeft() const
	{
		return _left;
	}

	void Viewport::_setTop(const int top)
	{
		_top = top;
	}

	int Viewport::_getTop() const
	{
		return _top;
	}

	void Viewport::_setWidth(const int width)
	{
		_width = width;
	}

	int Viewport::_getWidth() const
	{
		return _width;
	}

	void Viewport::_setHeight(const int height)
	{
		_height = height;
	}

	int Viewport::_getHeight() const
	{
		return _height;
	}
};
