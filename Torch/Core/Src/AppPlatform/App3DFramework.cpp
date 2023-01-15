#include <Torch/Torch.hpp>
#include <TML/Util.hpp>
#include <Torch/Interfaces/Context.hpp>
#include <Torch/Interfaces/Window.hpp>
#include <assert.h>
#include <Torch/Interfaces/App3DFramework.hpp>

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
        Context::Instance().setApp(this);

        auto cfg = Context::Instance().getConfig();

        if(cfg._deferred_rendering)
        {
            //TODO: register deferred rendering module.
        }

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

    WindowPtr App3DFramework::MakeWindow(std::string const &name)
    {
        RenderSettings setting;
        return MakeSharedPtr<Window>(name, setting, nullptr);
    }

    WindowPtr App3DFramework::MakeWindow(std::string const &name, void *native_wnd)
    {
        RenderSettings setting;
        return MakeSharedPtr<Window>(name, setting, native_wnd);
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
