#include <Torch/Torch.hpp>
#include <Torch/Interfaces/App3DFramework.hpp>
#include <Torch/Interfaces/Window.hpp>
#include <glm/glm.hpp>
#include <TML/Util.hpp>
#include <glad.h>

#include <algorithm>
#include <cstring>
#include <ostream>
#include <string>

#include <OpenGL/OGLRenderEngine.hpp>
#include <OpenGL/OGLCanvas.hpp>

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
        static std::wstring name(L"OpenGL Render Engine.");
        return name;
// wglMakeCurrent
    
    }

    void OGLRenderEngine::_CreateRenderWindow(std::string const& name, RenderSettings const& settings)
    {
        auto win = MakeSharedPtr<OGLCanvas>(name, settings);
    }

};