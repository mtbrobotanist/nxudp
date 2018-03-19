//
// Created by n8 on 3/18/18.
//

#include "int_buffer.h"

namespace nxudp
{
    namespace utils
    {
        void populate_buffer(int value, int_buffer& out_buf)
        {
            std::size_t size = sizeof(int);

            for (short i = 0; i < size; ++i)
            {
                out_buf[i] = value >> (i * 8);
            }
        }

        int buffer_to_value(const int_buffer& in_buffer)
        {
            int result = 0;
            std::size_t size = sizeof(int);

            for(short i = 0; i < size; ++i)
            {
                result |= in_buffer[i] << i * 8;
            }

            return result;
        }
    }
}