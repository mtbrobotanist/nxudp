//
// Created by n8 on 3/18/18.
//

#include "int_buffer.h"

namespace nxudp
{
namespace utils
{

std::size_t INT_SIZE = sizeof(int);

void write_buffer(int value, int_buffer &out_buffer)
{
    for(std::size_t i = 0; i < INT_SIZE; ++i)
    {
        out_buffer[i] = value >> (i * 8);
    }
}

int read_buffer(const int_buffer &in_buffer)
{
    int result = 0;

    for(std::size_t i = 0; i < INT_SIZE; ++i)
    {
        result |= in_buffer[i] << (i * 8);
    }

    return result;
}

} // namespace utils
} // namespace nxudp