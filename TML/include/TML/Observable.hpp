#ifndef __TML_OBSERVABLE_HPP__
#define __TML_OBSERVABLE_HPP__

#pragma once
#include <TML/Util.hpp>
#include <TML/EventState.hpp>
#include <TML/Observer.hpp>

namespace Torch
{
    template<class T> 
    class Observable
    {
    public:
        using CallbackFunc = std::function<void(T *eventData, EventState &eventState)>;
        using SPTR = std::shared_ptr<Observable<T>>;

    private:
        std::vector<typename Observer<T>::Ptr> _observers;
        EventState _event_state;
        std::function<void(const typename Observer<T>::Ptr &observer)> _on_observer_added;

    public:
        Observable() : Observable{nullptr}
        {

        }

        Observable(const std::function<void(const typename Observer<T>::Ptr& observer)>& onObserverAdded)
        : _event_state{0}
        , _on_observer_added{onObserverAdded}
        {
        }

        Observable& operator=(const Observable&) = delete;

        virtual ~Observable() = default;

        std::vector<typename Observer<T>::Ptr>& observers()
        {
            return _observers;
        }

        operator bool() const
        {
            return !_observers.empty();
        }

        typename Observer<T>::Ptr Add(const CallbackFunc& callback, int mask = -1,
                                bool insertFirst = false, void** scope = nullptr,
                                bool unregisterOnFirstCall = false)
        {
            if (!callback) 
            {
                return nullptr;
            }

            const auto observer = std::make_shared<Observer<T>>(callback, mask, scope);
            observer->_unregister_on_next_call = unregisterOnFirstCall;

            if (insertFirst) 
            {
                _observers.insert(_observers.begin(), observer);
            }
            else 
            {
                _observers.emplace_back(observer);
            }

            if (_on_observer_added) {
                _on_observer_added(observer);
            }

            return observer;
        }

        typename Observer<T>::Ptr Add(CallbackFunc&& callback, int mask = -1, bool insertFirst = false,
                                    void** scope = nullptr, bool unregisterOnFirstCall = false)
        {
            if (!callback) 
            {
                return nullptr;
            }

            const auto observer = std::make_shared<Observer<T>>(std::move(callback), mask, scope);
            observer->_unregister_on_next_call = unregisterOnFirstCall;

            if (insertFirst) 
            {
                _observers.insert(_observers.begin(), observer);
            }
            else 
            {
                _observers.emplace_back(observer);
            }

            if (_on_observer_added) 
            {
                _on_observer_added(observer);
            }

            return observer;
        }

        typename Observer<T>::Ptr AddOnce(CallbackFunc&& callback)
        {
            return add(callback, -1, false, nullptr, true);
        }

        bool Remove(const typename Observer<T>::Ptr& observer)
        {
            if (!observer) {
            return false;
            }

            const auto it = std::find_if(_observers.begin(), _observers.end(),
                            [observer](const typename Observer<T>::Ptr& obs) { return obs == observer; });

            if (it != _observers.end()) 
            {
                _DeferUnregister(*it);
                return true;
            }
            return false;
        }

        void MakeObserverTopPriority(const typename Observer<T>::Ptr& observer)
        {
            _remove(observer);
            _observers.insert(_observers.begin(), observer);
        }

        void MakeObserverBottomPriority(const typename Observer<T>::Ptr& observer)
        {
            _remove(observer);
            _observers.emplace_back(observer);
        }

         bool notifyObservers(T* eventData = nullptr, int mask = -1, void** target = nullptr,
                            void** currentTarget = nullptr, void* userInfo = nullptr)
        {
            if (_observers.empty())
            {
                return true;
            }

            auto &state = _event_state;
            state._mask = mask;
            state._target = target;
            state._current_target = currentTarget;
            state._skip_next_observers = false;
            state._last_return_value = eventData;
            state._user_info = userInfo;

            for (const auto &obs : _observers)
            {
                if (obs->_will_be_unregistered)
                {
                    continue;
                }

                if (obs->_mask & mask)
                {
                    obs->_call_back(eventData, state);

                    if (obs->_unregister_on_next_call)
                    {
                        _deferUnregister(obs);
                    }
                }
                if (state._skip_next_observers)
                {
                    return false;
                }
            }
            return true;
        }

        void NotifyObserver(const typename Observer<T>::Ptr& observer, T* eventData = nullptr,int mask = -1)
        {
            if(observer->_will_be_unregistered)
            {
                return;
            }

            auto &state = _event_state;
            state._mask = mask;
            state._skip_next_observers = false;

            observer->_call_back(eventData, state);

            if (observer->_unregister_on_next_call)
            {
                _DeferUnregister(observer);
            }
        }

        bool HasObservers() const
        {
            return !_observers.empty();
        }

        void Clear()
        {
            _observers.clear();
            _on_observer_added = nullptr;
        }

        SPTR clone() const
        {
            auto const result = MakeSharedPtr<Observable<T>>();
            result->_observers = _observers;

            return result;
        }

        bool HasSpecificMask(int mask = -1)
        {
            for (const auto& obs : _observers) 
            {
                if (obs->_mask & mask || obs->_mask == mask) 
                {
                    return true;
                }
            }
            return false;
        }

    private:
        void _DeferUnregister(const typename Observer<T>::Ptr& observer)
        {
            observer->_unregister_on_next_call = false;
            observer->_will_be_unregistered  = true;
            _remove(observer);
        }

        bool _Remove(const typename Observer<T>::Ptr& observer)
        {
            if (!observer) 
            {
                return false;
            }

            const auto index = std::find(_observers.begin(), _observers.end(), observer);

            if (index != _observers.end()) 
            {
                _observers.erase(index);
                return true;
            }

            return false;
        }


    };
};

#endif