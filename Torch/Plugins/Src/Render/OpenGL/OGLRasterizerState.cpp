#include <OpenGL/OGLRasterizerState.hpp>

namespace Torch
{

    OGLRasterizerState::OGLRasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc)
        : RasterizerState{rdesc, ddesc, bdesc}
    {
    }

    void OGLRasterizerState::Active()
    {
    }

    void OGLRasterizerState::ForceDefaultState()
    {
    }

    OGLSamplerState::OGLSamplerState(const SamplerDesc &desc)
        : SamplerState{desc}
    {
    }

    OGLSamplerState::~OGLSamplerState()
    {
    }
};