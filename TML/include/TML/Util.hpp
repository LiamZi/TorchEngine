#ifndef __TML_INCLUDE_TML_UTIL_HPP
#define __TML_INCLUDE_TML_UTIL_HPP

#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <memory>

#include <assert.h>

#include <TML/Log.hpp>

namespace Torch
{

    std::wstring &Convert(std::wstring &dest, std::wstring_view src);

    std::wstring& Convert(std::wstring& dest, std::string_view src);

    template <typename T, typename... Args>
    inline std::unique_ptr<T> MakeUniquePtrHelper(std::false_type, Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    inline std::shared_ptr<T> MakeSharedPtr(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    inline std::unique_ptr<T> MakeUniquePtrHelper(std::true_type, size_t size)
    {
        static_assert(0 == std::extent<T>::value, "make_unique<T[N]() is forbidden, please use make_unique<T[]>().");

        return std::make_unique<T>(size);
    }

    template<typename T, typename... Args>
    inline std::unique_ptr<T> MakeUniquePtr(Args&&... args)
    {
        return MakeUniquePtrHelper<T>(std::is_array<T>(), std::forward<Args>(args)...);
    }

};  

#endif