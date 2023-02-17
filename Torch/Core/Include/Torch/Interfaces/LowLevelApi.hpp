#ifndef __TORCH_CORE_INTERFACES_LOW_LEVEL_API_HPP__
#define __TORCH_CORE_INTERFACES_LOW_LEVEL_API_HPP__

#pragma once


namespace Torch
{
    class LowLevelApi
    {

        
    public:
        virtual ~LowLevelApi() noexcept {};

        virtual std::wstring const & Name() const = 0;

    };
};

#endif