#include <TML/TML.hpp>
#include <TML/Log.hpp>

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
    class MultiOStreamCallback 
    {
    private:
        std::span<std::ostream *> _oss;

    protected:
        MultiOStreamCallback() = default;
        ~MultiOStreamCallback() = default;

    public:
        explicit MultiOStreamCallback(std::span<std::ostream *> oss)
        : _oss(oss)
        {

        }
        MultiOStreamCallback(const MultiOStreamCallback &) = delete;
        void operator=(const MultiOStreamCallback &) = delete;

    };

    std::ostream Log()
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

        // static CallbackOutputSteramBuf<MultiOStreamCallback> log_stream_buff
        // static std::ostream log_stream("log");
        // return log_stream;
    }
};
