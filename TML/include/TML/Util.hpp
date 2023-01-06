#ifndef __TML_INCLUDE_TML_UTIL_HPP
#define __TML_INCLUDE_TML_UTIL_HPP

#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <memory>

#include <assert.h>

#include <TML/Log.hpp>

namespace Torch
{

    std::wstring &Convert(std::wstring &dest, std::wstring_view src);

    std::wstring& Convert(std::wstring& dest, std::string_view src);

    template <typename T, typename... Args>
    inline std::unique_ptr<T> MakeUniquePtrHelper(std::false_type, Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    inline std::shared_ptr<T> MakeSharedPtr(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    inline std::unique_ptr<T> MakeUniquePtrHelper(std::true_type, size_t size)
    {
        static_assert(0 == std::extent<T>::value, "make_unique<T[N]() is forbidden, please use make_unique<T[]>().");

        return std::make_unique<T>(size);
    }

    template<typename T, typename... Args>
    inline std::unique_ptr<T> MakeUniquePtr(Args&&... args)
    {
        return MakeUniquePtrHelper<T>(std::is_array<T>(), std::forward<Args>(args)...);
    }

    template<class T>
    void UNUSED_PARAM(T const &)
    {
    }

    template<typename C, typename T, typename E = void>
    class ReadOnlyProperty;

    template<typename C, typename T>
    class ReadOnlyProperty<C, T, typename std::enable_if<std::is_fundamental<T>::value || std::is_same<std::string, T>::value>::type>
    {
    public:
        using Tgetter = T (C::*)() const;
    private:
        C *const _object;
        Tgetter const _getter;

    public:
       
        ReadOnlyProperty(C *propObject, Tgetter propGetter)
        : _object{propObject}
        , _getter{propGetter}
        {

        }

        ReadOnlyProperty &operator=(const ReadOnlyProperty &) = delete;
        ReadOnlyProperty(const ReadOnlyProperty &) = delete;

        operator T() const
        {
            return (_object->*_getter)();
        }

        T operator()() const
        {
            return (_object->*_getter)();
        }
    };

    template<typename C, typename T>
    class ReadOnlyProperty<C, T, typename std::enable_if<!std::is_fundamental<T>::value && !std::is_same<std::string, T>::value>::type>
    {
    public:
        using Tgetter = T &(C::*)();

    private:
        C *const _object;
        Tgetter const _getter;

    public:
        ReadOnlyProperty(C *propObject, Tgetter propGetter)
        : _object{propObject}
        , _getter{propGetter}
        {

        }

        ReadOnlyProperty &operator=(const ReadOnlyProperty &) = delete;
        ReadOnlyProperty(const ReadOnlyProperty &) = delete;

        operator const T &() const
        {
            return (_object->*_getter)();
        }

        operator T&() const
        {
            return (_object->*_getter)();
        }

        const T &operator()()const
        {
            return (_object->*_getter)();
        }

        T &operator()() const
        {
            return (_object->*_getter)();
        }
    };

    template<typename C, typename T, typename E = void>
    class WriteOnlyProperty;

    template<typename C, typename T>
    class WriteOnlyProperty<C, T, typename std::enable_if<std::is_fundamental<T>::value || std::is_same<std::string, T>::value>::type>
    {
    public:
        using Tsetter = void (C::*)(T);

    private:
        C *const _object;
        Tsetter const _setter;

    public:
        WriteOnlyProperty(C *propObject, Tsetter propSetter)
        : _object{propObject}
        , _setter{propSetter}
        {

        }

        WriteOnlyProperty &operator=(const WriteOnlyProperty &) = delete;
        WriteOnlyProperty(const WriteOnlyProperty &) = delete;

        C &operator=(T value)
        {
            (_object->*_setter)(value);
            return *_object;
        }
    };

    template<typename C, typename T>
    class WriteOnlyProperty<C, T, typename std::enable_if<!std::is_fundamental<T>::value || !std::is_same<std::string, T>::value>::type>
    {
    public:
        using Tsetter = void (C::*)(const T &);

    private:
        C *const _object;
        Tsetter const _setter;

    public:
         WriteOnlyProperty(C *propObject, Tsetter propSetter)
        : _object{propObject}
        , _setter{propSetter}
        {

        }

        WriteOnlyProperty &operator=(const WriteOnlyProperty &) = delete;
        WriteOnlyProperty(const WriteOnlyProperty &) = delete;

        C &operator=(const T &value)
        {
            (_object->*_setter)(value);
            return *_object;
        }
    };

    template <typename C, typename T, typename E = void>
    class Property; // undefined

    template <typename C, typename T>
    class Property<C, T, typename std::enable_if<std::is_fundamental<T>::value || std::is_same<std::string, T>::value>::type>
    {
    public:
        using Tgetter = T (C::*)() const;
        using Tsetter = void (C::*)(T);

    private:
        C *const _object;
        T C::*_attribute;
        Tgetter const _getter;
        Tsetter const _setter;

    public:
        Property(C *propObject, T C::*attribute)
        : _object{propObject}
        , _attribute{attribute}
        , _getter{nullptr}
        , _setter{nullptr}
        {
        }

        Property(C *proObject, Tgetter propGetter, Tsetter propSetter)
        : _object{proObject}
        , _attribute{nullptr}
        , _getter{propGetter}
        , _setter{propSetter}
        {
        }

        Property &operator=(const Property &) = delete;
        Property(const Property &) = delete;

        operator T() const
        {
            return _attribute ? (_object->*_attribute) : (_object->*_getter)();
        }

        T operator()() const
        {
            return _attribute ? (_object->*_attribute) : (_object->*_getter)();
        }

        C &operator=(T value)
        {
            if(_attribute)
            {
                (_object->*_attribute) = value;
            }
            else
            {
                (_object->*_setter)(value);
            }

            return *_object;
        }
    };

    template<typename C, typename T>
    class Property<C, T, typename std::enable_if<!std::is_fundamental<T>::value && !std::is_same<std::string, T>::value>::type>
    {
    public:
        using Tsetter = void (C::*)(const T &);
        using Tgetter = T &(C::*)();

    private:
        C *const _object;
        T C::*_attribute;
        Tgetter const _getter;
        Tsetter const _setter;

    public:
        Property(C *proObject, T C::*attribute)
        : _object{proObject}
        , _attribute{attribute}
        , _getter{nullptr}
        , _setter{nullptr}
        {
        }

        Property(C *proObject, Tgetter propGetter, Tsetter propSetter)
        : _object{proObject}
        , _attribute{nullptr}
        , _getter{propGetter}
        , _setter{propSetter}
        {
        }


        Property &operator=(const Property &) = delete;
        Property(const Property &) = delete;

        operator T &() const
        {
            return _attribute ? (_object->*_attribute) : (_object->*_getter)();
        }

        operator const T &() const
        {
            return _attribute ? (_object->*_attribute) : (_object->*_getter)(); 
        }

        T &operator()()
        {
            return _attribute ? (_object->*_attribute) : (_object->*_getter)();
        }

        const T &operator()() const
        {
            return _attribute ? (_object->*_attribute) : (_object->*_getter)();
        }

        C &operator=(T value)
        {
            if(_attribute)
            {
                (_object->*_attribute) = value;
            }
            else
            {
                (_object->*_setter)(value);
            }

            return *_object;
        }

    };
};  

#endif