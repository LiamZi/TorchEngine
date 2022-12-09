#include <Torch/Torch.hpp>
#include <TML/Util.hpp>
#include <Torch/Context.hpp>
#include <Torch/Window.hpp>
#include <assert.h>
#include <Torch/App3DFramework.hpp>

namespace Torch
{
    App3DFramework::App3DFramework(std::string const &name)
        : App3DFramework{name, nullptr}
    {
    }

    App3DFramework::App3DFramework(std::string const &name, void *native_wnd)
        :_name{name}
        ,_total_num_frames{0}
        ,_fps{0}
        ,_accumulate_time{0}
        ,_num_frames{0}
    {
        _main_wnd = this->MakeWindow(_name, native_wnd);
    }

    App3DFramework::~App3DFramework()
    {
        this->Destroy();
    }

    void App3DFramework::Create()
    {
    }

    void App3DFramework::Destroy()
    {
        _main_wnd.reset();
    }

    void App3DFramework::Suspend()
    {
    }

    void App3DFramework::Resume()
    {
    }

    void App3DFramework::Refresh()
    {
    }

    std::shared_ptr<Window> App3DFramework::MakeWindow(std::string const &name)
    {
        return MakeSharedPtr<Window>(name, nullptr);
    }

    std::shared_ptr<Window> App3DFramework::MakeWindow(std::string const &name, void *native_wnd)
    {
        return MakeSharedPtr<Window>(name, native_wnd);
    }

    void App3DFramework::Run()
    {
#if defined TORCH_PLATFORM_WINDOWS_DESKTOP
        bool gotMsg;
        MSG msg;

        ::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);

        while (WM_QUIT != msg.message)
        {
            if(_main_wnd->isActive())
            {
                gotMsg = (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0);
            }
            else
            {
                gotMsg = (::GetMessage(&msg, nullptr, 0, 0)  != 0);
            }

            if(gotMsg)
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                //render engine refresh
            }
        }        
#endif
        // this->OnDestroy();
    }

    void App3DFramework::Quit()
    {
#ifdef TORCH_PLATFORM_WINDOWS_DESKTOP
        ::PostQuitMessage(0);
#else
        exit(0);
#endif
    }

    void App3DFramework::OnResize(uint32_t width, uint32_t height)
    {
    }
};
