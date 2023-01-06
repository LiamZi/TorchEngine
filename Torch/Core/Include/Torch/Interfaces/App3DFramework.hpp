#ifndef __TORCH_CORE_APP_3D_FRAMEWORK_HPP__
#define __TORCH_CORE_APP_3D_FRAMEWORK_HPP__

#pragma once

#include <Torch/Torch.hpp>

#include <Torch/Interfaces/Window.hpp>



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
    
    public:
        explicit App3DFramework(std::string const &name);
        App3DFramework(std::string const &name, void *native_wnd);
        virtual ~App3DFramework();

        void Create();
        void Destroy();
		void Suspend();
		void Resume();
		void Refresh();

        std::string const & Name() const
		{
			return _name;
		}

		std::shared_ptr<Window> MakeWindow(std::string const & name);
		std::shared_ptr<Window> MakeWindow(std::string const & name, void* native_wnd);
		std::shared_ptr<Window> const & MainWnd() const
		{
			return _main_wnd;
		}

        void Run();
        void Quit();
        virtual void OnResize(uint32_t width, uint32_t height);
    };
};

#endif