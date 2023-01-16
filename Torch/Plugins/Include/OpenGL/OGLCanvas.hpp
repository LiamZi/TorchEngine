#ifndef __TORCH_PLUGINS_OGL_CANVAS_HPP__
#define __TORCH_PLUGINS_OGL_CANVAS_HPP__


#pragma once

#include <Torch/Interfaces/RenderSettings.hpp>
#include <OpenGL/OGLFrameBuffer.hpp>

namespace Torch
{
	class OGLCanvas final : public OGLFrameBuffer
	{
	public:
		OGLCanvas(std::string const &name, RenderSettings const &settings);
		virtual ~OGLCanvas() override;
	};
};

#endif	
