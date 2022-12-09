#ifndef __TORCH_SAMPLES_SRC_COMMON_COMMON_HPP__
#define __TORCH_SAMPLES_SRC_COMMON_COMMON_HPP__

#pragma once

//#ifndef SAMPLE_COMMON_SOURCE
//#ifdef TORCH_COMPILER_MSVC
//extern "C"
//{
//	TORCH_SYMBOL_EXPORT uint32_t NvOptimusEnablement = 0x00000001;
//}
//#endif
//#endif

int SampleMain();

inline int EntryFunc()
{
    return SampleMain();
}

#endif