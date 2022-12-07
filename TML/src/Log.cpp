#include <TML/TML.hpp>
#include <TML/Log.hpp>

#include <cstdarg>
#include <cstdio>
#include <iostream>


class MultiOStreamCallback 
{
protected:
    MultiOStreamCallback() = default;
    ~MultiOStreamCallback() = default;

public:
    MultiOStreamCallback(const MultiOStreamCallback &) = delete;
    void operator=(const MultiOStreamCallback &) = delete;
};