#include <Torch/Interfaces/LowLevelApi.hpp>
#include <Torch/Renderer/RasterizerState.hpp>
#include <TML/Hash.hpp>

namespace Torch
{
    RasterizerStatePtr LowLevelApi::CreateRasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc)
    {
        RasterizerStatePtr ret;

        const char *rsDescBegin = reinterpret_cast<const char *>(&rdesc);
        const char *rsDescEnd = rsDescBegin + sizeof(rdesc);

        const char *dDescBegin= reinterpret_cast<const char *>(&ddesc);
        const char *dDescEnd = dDescBegin + sizeof(ddesc);

        const char *bDescBegin = reinterpret_cast<const char *>(&bdesc);
        const char *bDescEnd = bDescBegin + sizeof(bdesc);

        size_t seed = HashRange(rsDescBegin, rsDescEnd);
        HashRange(dDescBegin, dDescEnd);
        HashRange(bDescBegin, bDescEnd);

        auto iter = _rasterizers.find(seed);
        if(iter != _rasterizers.end())
        {
            ret = _CreateRasterizerState(rdesc, ddesc, bdesc);
            _rasterizers.emplace(seed, ret);
        }
        else
        {
            ret = iter->second;
        }

        return ret;
    }


};
