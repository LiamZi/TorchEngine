#ifndef __TORCH_CORE_INTERFACES_LOW_LEVEL_API_HPP__
#define __TORCH_CORE_INTERFACES_LOW_LEVEL_API_HPP__

#pragma once

#include <Torch/Torch.hpp>
#include <string>
#include <unordered_map>

namespace Torch
{
    FWD_STRUCT_SPTR(RasterizerDesc)
    FWD_STRUCT_SPTR(DepthStencilDesc)
    FWD_STRUCT_SPTR(BlendDesc)
    FWD_STRUCT_SPTR(SamplerDesc)
    FWD_CLASS_SPTR(RasterizerState)
    FWD_CLASS_SPTR(SamplerState)


    class LowLevelApi
    {
    protected:
        std::unordered_map<size_t, RasterizerStatePtr> _rasterizers;
        std::unordered_map<size_t, SamplerStatePtr> _samplerStates;

    public:

        virtual ~LowLevelApi() noexcept {};

        virtual std::wstring const & Name() const = 0;

    public:
        RasterizerStatePtr CreateRasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc);

    private:
        virtual RasterizerStatePtr _CreateRasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc) = 0;
    };
};

#endif