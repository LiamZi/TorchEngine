#ifndef __TORCH_CORE_APP_3D_FRAMEWORK_HPP__
#define __TORCH_CORE_APP_3D_FRAMEWORK_HPP__

#pragma once

#include <Torch/Torch.hpp>

namespace Torch
{
    class TORCH_CORE_API App3DFramework
    {
    protected:
        std::string _name;

        uint32_t _total_num_frames;
        float _fps;
        float _accumulate_time;
        uint32_t _num_frames;

        std::shared_ptr<Window> _main_wnd;

    };
};

#endif