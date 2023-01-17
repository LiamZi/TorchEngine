#ifndef __TML_EVENT_STATE_HPP__
#define __TML_EVENT_STATE_HPP__

#pragma once

#include <memory>


namespace Torch
{
    class EventState
    {
    public:
        bool _skip_next_observers;
        int _mask;
        void **_target;
        void **_current_target;
        void *_last_return_value;
        void *_user_info;

    public:
        EventState(int mask, bool skipNextObservers = false, void **target = nullptr, void **currentTarget = nullptr);
        EventState(const EventState &other);
        EventState(EventState &&other);
        EventState &operator=(const EventState &other);
        EventState &operator=(EventState &&other);
        ~EventState();

        EventState& initialize(int mask, bool skipNextObservers = false, void** target = nullptr, void** currentTarget = nullptr);
    };
};

#endif