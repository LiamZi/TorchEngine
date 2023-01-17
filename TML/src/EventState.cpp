#include <TML/EventState.hpp>

namespace Torch
{

    EventState::EventState(int mask, bool skipNextObservers, void **target, void **currentTarget)
    {
        initialize(mask, skipNextObservers, target, currentTarget);
    }

    EventState::EventState(const EventState &other) = default;

    EventState::EventState(EventState &&other) = default;

    EventState &EventState::operator=(const EventState &other) = default;

    EventState &EventState::operator=(EventState &&other) = default;

    EventState::~EventState() = default;

    EventState &EventState::initialize(int mask, bool skipNextObservers, void **target, void **currentTarget)
    {
        _mask = mask;
        _skip_next_observers = skipNextObservers;
        _target = target;
        _current_target = currentTarget;
        _last_return_value = nullptr;
        _user_info = nullptr;

        return *this;
    }
};