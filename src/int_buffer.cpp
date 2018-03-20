//
// Created by n8 on 3/18/18.
//

#include "int_buffer.h"

namespace nxudp
{
    namespace utils
    {
        std::size_t int_size = sizeof(int);
        
        void populate_buffer(int value, int_buffer& out_buffer)
        {
            for(std::size_t i = 0; i < int_size; ++i)
            {
                out_buffer[i] = value >> (i * 8);
            }
        }

        int buffer_to_value(const int_buffer& in_buffer)
        {
            int result = 0;
            
            for(std::size_t i = 0; i < int_size; ++i)
            {
                result |= in_buffer[i] << (i * 8);
            }

            return result;
        }
    }
}