#include <OpenGL/OGLCanvas.hpp>


namespace Torch
{
	OGLCanvas::OGLCanvas(std::string const& name, RenderSettings const& settings)
		:OGLFrameBuffer(false)
	{
	}

	OGLCanvas::~OGLCanvas() = default;
};
