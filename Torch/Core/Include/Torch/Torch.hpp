#ifndef __TORCH_HPP__
#define __TORCH_HPP__

#pragma once

#define TORCH_NAME Torch
#define TORCH_VER_MAJOR 1
#define TORCH_VER_MINOR 1
#define TORCH_VER_RELEASE 0
#define TORCH_VER_STR KFL_STRINGIZE(KLAYGE_NAME) " " KFL_STRINGIZE(KLAYGE_VER_MAJOR) "." KFL_STRINGIZE(KLAYGE_VER_MINOR) "." KFL_STRINGIZE(KLAYGE_VER_RELEASE)


#ifdef TORCH_PLATFORM_WINDOWS
#define KLAYGE_SYMBOL_EXPORT __declspec(dllexport)
#define KLAYGE_SYMBOL_IMPORT __declspec(dllimport)
#else
#define TORCH_SYMBOL_EXPORT __attribute__((visibility("default")))
#define TORCH_SYMBOL_IMPORT
#endif


#ifdef TORCH_CORE_SOURCE		// Build dll
	#define KLAYGE_CORE_API TORCH_SYMBOL_EXPORT
#else							// Use dll
	#define TORCH_CORE_API TORCH_SYMBOL_IMPORT
#endif

#include <vector>
#include <string>
#include <Torch/Context.hpp>

#endif