#ifndef __TML_LOG_HPP__
#define __TML_LOG_HPP__

#include <iosfwd>

#pragma once

namespace Torch
{
    std::ostream &LogDebug();
    std::ostream &LogInfo();
    std::ostream &LogWarn();
    std::ostream &LogError();
};

#endif