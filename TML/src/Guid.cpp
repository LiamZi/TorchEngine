#include <TML/Guid.hpp>
#include <TML/Random.hpp>
#include <array>

namespace Torch
{
    std::string GUID::RandomId()
    {
        std::string random_id = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
        const std::array<unsigned int, 16> yconv
        {
           {8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11} 
        };

        const std::array<char, 16> hex
        {
            {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'}
        };

        const auto random_floats = Math::RandomList(0.f, 1.f, random_id.size());
        for(unsigned int i = 0; i < random_id.size(); ++i)
        {
            const auto c = random_id[i];
            if(c == 'x' || c == 'y')
            {
                const auto r = static_cast<unsigned int>(random_floats[i] * 16);
                random_id[i] = (c == 'x') ? hex[r] :  hex[yconv[r]];
            }
        }

        return random_id;
    }
};