#include <Torch/Torch.hpp>
#include <Torch/Interfaces/Context.hpp>

#include <OpenGL/OGLRenderEngine.hpp>
#include <OpenGL/OGLFrameBuffer.hpp>
#include <glad.h>


namespace Torch
{
	OGLFrameBuffer::OGLFrameBuffer(bool offScreen)
	{
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
};
