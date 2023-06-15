#include <Torch/Renderer/RasterizerState.hpp>

namespace Torch
{
    RasterizerState::RasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc)
        : _rasterizerDesc{rdesc}
        , _depthDesc{ddesc}
        , _blendDesc{bdesc}
    {
    }

    RasterizerState::~RasterizerState() = default;


    SamplerState::SamplerState(const SamplerDesc &desc)
        : _desc{desc}
    {
    }
};
