#ifndef __TORCH_PLUGINS_OGL_LOW_LEVEL_API_HPP__
#define __TORCH_PLUGINS_OGL_LOW_LEVEL_API_HPP__

#pragma once


#include <Torch/Interfaces/LowLevelApi.hpp>


namespace Torch
{
    class OGLLowLevelApi final : public LowLevelApi
    {
    private:
#if defined TORCH_PLATFORM_WINDOWS
        typedef HGLRC(WINAPI* wglCreateContextFunc)(HDC hdc);
        typedef BOOL(WINAPI* wglDeleteContextFunc)(HGLRC hglrc);
        typedef BOOL(WINAPI* wglMakeCurrentFunc)(HDC hdc, HGLRC hglrc);

        DllLoader _opengl32;
        wglCreateContextFunc _dynamic_wgl_create_context;
        wglDeleteContextFunc _dynamic_wgl_delete_context;
        wglMakeCurrentFunc _dynamic_wgl_make_current;

#endif

    public:
        OGLLowLevelApi();
  
        OGLLowLevelApi(OGLLowLevelApi const &) = delete;
        OGLLowLevelApi &operator=(OGLLowLevelApi const &) = delete;

        std::wstring const & Name() const override;

    private:
        RasterizerStatePtr _CreateRasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc) override;

    public:
#if defined TORCH_PLATFORM_WINDOWS
        HGLRC wglCreateContext(HDC hdc);
        BOOL wglDeleteContext(HGLRC hglrc);
        BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc);

#endif
    };
};

#endif