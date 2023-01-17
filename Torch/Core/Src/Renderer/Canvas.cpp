#include <Torch/Renderer/Canvas.hpp>
#include <Torch/Renderer/Viewport.hpp>

namespace Torch
{
	Canvas::Canvas()
		: Viewport(this, &Canvas::_getViewport, &Canvas::_setViewport)
	{
	}

	void Canvas::OnError(int errorId, const char* errorDesc)
	{
	}

	void Canvas::_setViewport(const ViewportPtr &viewport)
	{
	}

	ViewportPtr& Canvas::_getViewport()
	{
		return _viewport;
	}

	void Canvas::Focus()
	{
	}

	void Canvas::ApplicationDidEnterBackground()
	{
	}

	void Canvas::ApplicationDWillEnterForeground()
	{
	}

    void Canvas::Present()
    {
    }

    void Canvas::WaitOnPresent()
    {
    }
};
