#ifndef __TORCH_TML_GUID_HPP__
#define __TORCH_TML_GUID_HPP__

#include <string>
#include <TML/Platform.hpp>

namespace Torch
{
	struct TORCH_SYMBOL_EXPORT GUID
	{
		static std::string RandomId();
	};
};

#endif // !__TORCH_TML_GUID_HPP__
