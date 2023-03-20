#ifndef __TML_INCLUDE_TML_STREAM_BUFFER_HPP__
#define __TML_INCLUDE_TML_STREAM_BUFFER_HPP__

#pragma once

#include <streambuf>
#include <vector>
#include <string>


namespace Torch
{
    template<typename T>
    class CallbackOutputStreamBuffer : public std::streambuf
    {
    private:
        T _cb;

    public:
        explicit CallbackOutputStreamBuffer(T cb) noexcept
            :_cb{std::move(cb)}
        {

        }

        virtual ~CallbackOutputStreamBuffer() override
        {

        }

    protected:
        std::streamsize xsputn(const char_type *s, std::streamsize count) override
        {
            return _cb(s, count);
        }

		int_type overflow(int_type ch = traits_type::eof()) override
		{
			return _cb(&ch, 1);
		}
    };
}; // namespace Torch


#endif