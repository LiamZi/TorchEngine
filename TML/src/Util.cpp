#include <TML/TML.hpp>

#ifdef TORCH_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#include <TML/Util.hpp>

#include <vector>
#include <algorithm>
#include <assert.h>

namespace Torch
{
    std::wstring &Convert(std::wstring &dest, std::wstring_view src)
    {
#if defined TORCH_PLATFORM_WINDOWS
        int const mbs_len = WideCharToMultiByte(CP_ACP, 0, src.data(), static_cast<int>(src.size()), nullptr, 0, nullptr, nullptr);
        auto lhs = MakeUniquePtr<char[]>(mbs_len + 1);
        WideCharToMultiByte(CP_ACP, 0, src.data(), static_cast<int>(src.size()), lhs.get(), mbs_len, nullptr, nullptr);

#endif

        dest.assign(&lhs[0], &lhs[mbs_len]);

        return dest;

    }
}

std::wstring &Torch::Convert(std::wstring &dest, std::string_view src)
{
#if defined TORCH_PLATFORM_WINDOWS
    int const wcs_len = MultiByteToWideChar(CP_ACP, 0, src.data(), static_cast<int>(src.size()), nullptr, 0);
    auto lhs = MakeUniquePtr<wchar_t[]>(wcs_len + 1);
    MultiByteToWideChar(CP_ACP, 0, src.data(), static_cast<int>(src.size()), lhs.get(), wcs_len);
#endif

    dest.assign(&lhs[0], &lhs[wcs_len]);

    return dest;
}
