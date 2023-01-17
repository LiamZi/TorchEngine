#ifndef __TORCH_TML_OBSERVER_HPP__
#define __TORCH_TML_OBSERVER_HPP__

#pragma once

#include <functional>
#include <TML/EventState.hpp>

namespace Torch
{
    template <class T>
    class Observer
    {
    public:
        using CallbackFunc = std::function<void(T *eventData, EventState &eventState)>;
        using Ptr = std::shared_ptr<Observer<T>>;

    public:
        bool _will_be_unregistered;
        bool _unregister_on_next_call;
        CallbackFunc _call_back;
        int _mask;
        void **_scope;

    public:
        Observer()
            : _will_be_unregistered{false}
            , _unregister_on_next_call{false}
            , _call_back{nullptr}
            , _mask{-1}
            , _scope{nullptr}
        {
        }

        Observer(const CallbackFunc &callback, int mask, void **scope)
            : _will_be_unregistered{false}
            , _unregister_on_next_call{false}
			, _call_back{ callback }
            , _mask{mask}
            , _scope{scope}
        {
        }

        ~Observer() = default;

        operator bool() const
        {
            return (_call_back == nullptr) && (_mask == -1) && (_scope == nullptr);
        }
    };
};


#endif