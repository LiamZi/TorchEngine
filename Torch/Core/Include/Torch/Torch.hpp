#ifndef __TORCH_HPP__
#define __TORCH_HPP__

#pragma once

#include <TML/TML.hpp>



#define TORCH_NAME Torch
#define TORCH_VER_MAJOR 1
#define TORCH_VER_MINOR 1
#define TORCH_VER_RELEASE 0
#define TORCH_VER_STR TML_STRINGIZE(TORCH_NAME) " " TML_STRINGIZE(TORCH_VER_MAJOR) "." TML_STRINGIZE(TORCH_VER_MINOR) "." TML_STRINGIZE(TORCH_VER_RELEASE)

#define TORCH_COMPILER_TOOLSET TML_STRINGIZE(TML_JOIN(TORCH_COMPILER_NAME, TORCH_COMPILER_VERSION))

#ifdef TORCH_CORE_SOURCE
    #define TORCH_CORE_API TORCH_SYMBOL_EXPORT
#else
    #define TORCH_CORE_API TORCH_SYMBOL_IMPORT
#endif

#include <vector>
#include <string>
#include <Torch/Interfaces/Context.hpp>
#include <glad.h>

#endif