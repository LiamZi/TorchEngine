#ifndef __TORCH_PLUGINS_OGL_FRAME_BUFFER_HPP__
#define __TORCH_PLUGINS_OGL_FRAME_BUFFER_HPP__

#pragma once

#include <Torch/Renderer/Canvas.hpp>

namespace Torch
{
	FWD_CLASS_SPTR(Viewport);

	class OGLFrameBuffer : public Canvas
	{
	protected:
		ViewportPtr _viewport;

	public:
		explicit OGLFrameBuffer(bool offScreen);
		~OGLFrameBuffer() override;

		std::wstring const& Description() const override;

	public:
		ReadOnlyProperty<OGLFrameBuffer, ViewportPtr> View_port;

	private:
		ViewportPtr &_getViewport();

	};
};

#endif