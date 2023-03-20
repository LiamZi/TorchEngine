#include <TML/TML.hpp>
#include <TML/Log.hpp>
#include <TML/StreamBuffer.hpp>

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <glm/glm.hpp>

#ifdef TORCH_PLATFORM_ANDROID
#else
#include <fstream>
#endif

#include <span>

namespace
{
    using namespace Torch;
	class MultiOStreamsCallback
	{
    private:
		std::span<std::ostream*> _oss;
	public:
		explicit MultiOStreamsCallback(std::span<std::ostream*> oss)
			: _oss(oss)
		{
		}
		MultiOStreamsCallback(MultiOStreamsCallback&& rhs) noexcept
			: _oss(std::move(rhs._oss))
		{
		}

		std::streambuf::int_type operator()(void const * buff, std::streamsize count)
		{
			for (auto& os : _oss)
			{
				os->write(static_cast<char const *>(buff), count);
			}
			return static_cast<std::streambuf::int_type>(count);
		}
	};

    std::ostream &Log()
    {
#ifdef TORCH_DEBUG
        static std::ofstream log_file("Torch.log");
#endif
        static std::ostream *oss[] = 
        {
#ifdef TORCH_DEBUG
            &log_file,
#endif
            &std::clog
        };

        static CallbackOutputStreamBuffer<MultiOStreamsCallback> log_stream_buff((MultiOStreamsCallback(oss)));
        static std::ostream log_stream(&log_stream_buff);

        return log_stream;
    }
};


namespace Torch
{
    std::ostream &LogDebug()
    {
        return Log() << "[Debug] Torch: ";
    }

    std::ostream &LogInfo()
    {
        return Log() << "[INFO] Torch: ";
    }

    std::ostream &LogWarn()
    {
        return Log() << "[WARN] Torch: ";
    }

    std::ostream &LogError()
    {
        return Log() << "[ERROR] Torch: ";
    }
    
}; 

