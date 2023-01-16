#ifndef __TORCH_PLUGINS_OGL_FRAME_BUFFER_HPP__
#define __TORCH_PLUGINS_OGL_FRAME_BUFFER_HPP__

#pragma once

#include <Torch/Renderer/Canvas.hpp>

namespace Torch
{
	class OGLFrameBuffer : public Canvas
	{
	public:
		explicit OGLFrameBuffer(bool offScreen);
		~OGLFrameBuffer() override;

		std::wstring const& Description() const override;

	};
};

#endif