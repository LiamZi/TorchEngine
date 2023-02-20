#include <Torch/Torch.hpp>
#include <Torch/Interfaces/Context.hpp>
#include <Torch/Renderer/Viewport.hpp>

#include <OpenGL/OGLFrameBuffer.hpp>



namespace Torch
{
	OGLFrameBuffer::OGLFrameBuffer(bool offScreen)
		: View_port(this, &OGLFrameBuffer::_getViewport)
	{
		_viewport = MakeSharedPtr<Viewport>(0, 0, 0, 0);

		if(offScreen)
		{
			//glad_glCreateFramebuffers(1, &_fbo);
		}
		else
		{
		}

	}

	OGLFrameBuffer::~OGLFrameBuffer()
	{
	}

	std::wstring const& OGLFrameBuffer::Description() const
	{
		static std::wstring const desc(L"OpenGL Frame Buffer Object");
		return desc;
	}

	ViewportPtr& OGLFrameBuffer::_getViewport()
	{
		return _viewport;
	}
};
