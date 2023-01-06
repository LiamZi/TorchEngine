#include <Torch/Torch.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>
#include <Torch/Interfaces/Window.hpp>
#include <glm/glm.hpp>
#include <TML/Util.hpp>


#include <algorithm>
#include <cstring>
#include <ostream>
#include <string>

#include <OpenGL/OGLRenderEngine.hpp>

namespace Torch
{
    OGLRenderEngine::OGLRenderEngine()
    : Name{this, &OGLRenderEngine::_Name}
    {
    }

    OGLRenderEngine::~OGLRenderEngine()
    {
    }

    std::wstring &OGLRenderEngine::_Name()
    {
        // TODO: insert return statement here
        static std::wstring name(L"OpenGL Render Engine.");
        return name;
    }

    // std::wstring &OGLRenderEngine::_Name()
    // {
    //     static std::wstring name(L"OpenGL Render Engine.");
    //     return name;
    // }
};