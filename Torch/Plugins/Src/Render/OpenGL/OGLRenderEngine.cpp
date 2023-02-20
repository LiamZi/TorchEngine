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

        }

        this->SetRenderDeivceFeatures();
        this->InitRenderStates();
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
        Device_Feature()._cs_support = false;
        Device_Feature()._hs_support = true;
        Device_Feature()._ds_support = true;
        Device_Feature()._tessType = TT_HARDWARE;


    }


    void OGLRenderEngine::InitRenderStates()
    {
    }
};

extern "C"
{
    TORCH_SYMBOL_EXPORT void MakeRenderEngine(std::unique_ptr<Torch::Engine> &ptr)
    {
        ptr = Torch::MakeUniquePtr<Torch::OGLRenderEngine>();
    }
}