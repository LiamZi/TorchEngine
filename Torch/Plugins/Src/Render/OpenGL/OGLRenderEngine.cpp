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
#include <OpenGL/OGLLowLevelApi.hpp>

namespace
{
    const char *DebugSourceString(GLenum v)
    {
        const char *ret = "";
        switch (v)
        {
        case GL_DEBUG_SOURCE_API:
            ret = "GL";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            ret = "GL Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            ret = "Window system";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            ret = "3rd party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            ret = "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            ret = "Other";
            break;
        default:

            break;
        }
        return ret;
    }

    const char *DebugTypeString(GLenum v)
    {
        char const *ret = "";
        switch (v)
        {
        case GL_DEBUG_TYPE_ERROR:
            ret = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            ret = "Depercated behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            ret = "Undefined behavior";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            ret = "Performance";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            ret = "Portability";
            break;
        case GL_DEBUG_TYPE_MARKER:
            ret = "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            ret = "Push group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            ret = "Pop group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            ret = "Other";
            break;
        
        default:
            break;
        }

        return ret;
    }

    const char *DebugSeverityString(GLenum v)
    {
        const char *ret = "";
        switch (v)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            ret = "High";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            ret = "Medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            ret = "Low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            ret = "Notification";
            break;
        
        default:
            break;
        }
        return ret;
    }

    void GLAPIENTRY DebugOutputProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			GLchar const * message, void const * user_param)
    {
        (void)(length);
        (void)(user_param);

        auto &os = (GL_DEBUG_TYPE_ERROR == type) ? Torch::LogError() : Torch::LogInfo();
        os << "OpenGL debug output source:" << DebugSourceString(source) << "; "
            << "type: " << DebugTypeString(type) << "; "
            << "id: " << id << "; "
            << "severity: " << DebugSeverityString(severity) << "; "
            << "message: " << message << std::endl;
    }
};

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

        if(GLAD_GL_VERSION_4_6)
        {
            _shader_platform_name = "gl_4_6";
        }
        else if(GLAD_GL_VERSION_4_5)
        {
            _shader_platform_name = "gl_4_5";
        }
        else if(GLAD_GL_VERSION_4_4)
        {
            _shader_platform_name = "gl_4_4";
        }
        else if(GLAD_GL_VERSION_4_4)
        {
            _shader_platform_name = "gl_4_3";
        }
        else if(GLAD_GL_VERSION_4_2)
        {
            _shader_platform_name = "gl_4_2";
        }
        else
        {
            _shader_platform_name = "gl_4_1";
        }

        this->SetRenderDeivceFeatures();
        this->InitPipelineStates();

#ifdef TORCH_DEBUG
        bool const debug_context = true;
#else
        bool const debug_context = settings._debug_context;
#endif
        if(debug_context)
        {
            if(GLAD_GL_VERSION_4_3 || GLAD_GL_KHR_debug)
            {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(&DebugOutputProc, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

            }
            else if(GLAD_GL_ARB_debug_output)
            {
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
                glDebugMessageCallback(&DebugOutputProc, nullptr);
                 glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH_ARB, 0, nullptr, GL_TRUE);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM_ARB, 0, nullptr, GL_TRUE);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, nullptr, GL_FALSE);
            }
        }

        // win->Attach()

        
    }

    std::unique_ptr<LowLevelApi> OGLRenderEngine::DoCreateLowLevelApi()
    {
        return MakeUniquePtr<OGLLowLevelApi>();
    }

    void OGLRenderEngine::SetRenderDeivceFeatures()
    {
        GLint tmp;

        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &tmp);
        Device_Feature()._max_vertex_texture_units = static_cast<uint8_t>(tmp);

        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tmp);
        Device_Feature()._max_texture_width = tmp;
        Device_Feature()._max_texture_height = tmp;

        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &tmp);
        Device_Feature()._max_texture_depth = tmp;

        glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &tmp);
        Device_Feature()._max_texture_cube_size = tmp;

        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, &tmp);
        Device_Feature()._max_texture_array_length = tmp;

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tmp);
        Device_Feature()._max_pixel_texture_units = tmp;

        glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &tmp);
        Device_Feature()._max_geometry_texture_units = tmp;

        if(GLAD_GL_VERSION_4_6 || GLAD_GL_ARB_texture_filter_anisotropic || GLAD_GL_EXT_texture_filter_anisotropic)
        {
            glGetIntegerv(GL_TEXTURE_MAX_ANISOTROPY, &tmp);
            Device_Feature()._max_texture_anisotropy = static_cast<uint8_t>(tmp);
        }
        else
        {
            Device_Feature()._max_texture_anisotropy = 1;
        }

        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &tmp);
        Device_Feature()._max_simultaneous_rts = static_cast<uint8_t>(tmp);
        Device_Feature()._max_simultaneous_uavs = 0;

        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &tmp);
        Device_Feature()._max_vertex_streams = tmp;

        Device_Feature()._is_tbdr = false;
        Device_Feature()._primitive_restart_support = true;
        Device_Feature()._multithread_rendering_support = false;
        Device_Feature()._multithread_res_createing_support = false;
        Device_Feature()._arbitrary_multithread_rendering_support = false;
        Device_Feature()._mrt_independent_bit_depths_support = false;
        Device_Feature()._logic_op_support = true;
        Device_Feature()._independent_blend_support = true;
        Device_Feature()._draw_indirect_support = true;
        Device_Feature()._no_overwrite_support = false;
        Device_Feature()._full_npot_texture_support = true;

        if(Device_Feature()._max_texture_array_length > 1)
        {
            Device_Feature()._render_to_texture_array_support = true;
        }
        else
        {
            Device_Feature()._render_to_texture_array_support = false;
        }

        Device_Feature()._explicit_multi_sample_support = false;
        Device_Feature()._load_from_buffer_support = true;
        Device_Feature()._uavs_at_every_stage_support = false;
        Device_Feature()._rovs_support = false;
        Device_Feature()._flexible_srvs_support = false;
        Device_Feature()._vp_rt_index_at_every_stage_support = GLAD_GL_NV_viewport_array2 | GLAD_GL_AMD_vertex_shader_layer;

        Device_Feature()._gs_support = true;
        Device_Feature()._cs_support = true;
        Device_Feature()._hs_support = true;
        Device_Feature()._ds_support = true;
        Device_Feature()._tessType = TT_HARDWARE;


    }


    void OGLRenderEngine::InitPipelineStates()
    {
        auto &engine = Context::Instance().Engine().LowLevelApiInstance();
    }
};

extern "C"
{
    TORCH_SYMBOL_EXPORT void MakeRenderEngine(std::unique_ptr<Torch::Engine> &ptr)
    {
        ptr = Torch::MakeUniquePtr<Torch::OGLRenderEngine>();
    }
}