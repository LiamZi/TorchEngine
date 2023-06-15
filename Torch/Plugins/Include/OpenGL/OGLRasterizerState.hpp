#ifndef __TORCH_PLUGINS_OGL_RASTERIZER_STATE_HPP__
#define __TORCH_PLUGINS_OGL_RASTERIZER_STATE_HPP__

#pragma once

#include <Torch/Torch.hpp>
#include <Torch/Renderer/RasterizerState.hpp>



namespace Torch
{
    class OGLRasterizerState final : public RasterizerState
    {
    private:
        GLenum _polygonMode;
        GLenum _shadeMode;
        GLenum _frontFace;
        GLenum _depthWriteMask;
        GLenum _depthFunc;
        GLenum _frontStencilFunc;
        GLenum _frontStencilFail;
        GLenum _frontStencilDepthFail;
        GLenum _frontStencilPass;
        GLenum _backStencilFunc;
        GLenum _backStencilFail;
        GLenum _backStencilDepthFail;
        GLenum _backStencilPass;

        GLenum _blendOp;
        GLenum _blendOpAlpha;
        GLenum _srcBlend;
        GLenum _dstBlend;
        GLenum _srcBlendAlpha;
        GLenum _dstBlendAlpha;
        GLenum _logicOp;

    public:
        OGLRasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc);
        void Active();
        void ForceDefaultState();
    };

    class OGLSamplerState final : public SamplerState
    {
    private:
        GLuint _sampler;
    
    public:
        explicit OGLSamplerState(const SamplerDesc &desc);
        ~OGLSamplerState() override;
    };
};


#endif