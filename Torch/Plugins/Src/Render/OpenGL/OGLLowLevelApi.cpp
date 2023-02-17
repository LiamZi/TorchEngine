#include <OpenGL/OGLLowLevelApi.hpp>


namespace Torch
{   
    OGLLowLevelApi::OGLLowLevelApi()
    {
#if defined TORCH_PLATFORM_WINDOWS
		_opengl32.Load("opengl32.dll");

		_dynamic_wgl_create_context = reinterpret_cast<wglCreateContextFunc>(_opengl32.GetProcAddress("wglCreateContext"));
		_dynamic_wgl_delete_context = reinterpret_cast<wglDeleteContextFunc>(_opengl32.GetProcAddress("wglDeleteContext"));
		_dynamic_wgl_make_current = reinterpret_cast<wglMakeCurrentFunc>(_opengl32.GetProcAddress("wglMakeCurrent"));
#endif

    }

    std::wstring const &OGLLowLevelApi::Name() const
    {
        static std::wstring const name(L"OpenGL Render Api");
        return name;
    }

	HGLRC OGLLowLevelApi::wglCreateContext(HDC hdc)
	{
		return _dynamic_wgl_create_context(hdc);
	}

	BOOL OGLLowLevelApi::wglDeleteContext(HGLRC hglrc)
	{
		return _dynamic_wgl_delete_context(hglrc);
	}

	BOOL OGLLowLevelApi::wglMakeCurrent(HDC hdc, HGLRC hglrc)
	{
		return _dynamic_wgl_make_current(hdc, hglrc);
	}
};


