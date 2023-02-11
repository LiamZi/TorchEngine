#ifndef __TORCH_PLUGINS_OGL_LOW_LEVEL_API_HPP__
#define __TORCH_PLUGINS_OGL_LOW_LEVEL_API_HPP__

#pragma once


#include <Torch/Interfaces/LowLevelApi.hpp>


namespace Torch
{
    class OGLLowLevelApi final : public LowLevelApi
    {
    public:
        OGLLowLevelApi();
  
        OGLLowLevelApi(OGLLowLevelApi const &) = delete;
        OGLLowLevelApi &operator=(OGLLowLevelApi const &) = delete;

        std::wstring const & Name() const override;   
    };
};

#endif