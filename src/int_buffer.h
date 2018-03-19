//
// Created by n8 on 3/18/18.
//

#ifndef NX_UDP_INT_BUFFER_H
#define NX_UDP_INT_BUFFER_H

#include <array>

namespace nxudp
{

typedef std::array<uint8_t, sizeof(int)> int_buffer;

namespace utils
{

void populate_buffer(int value, int_buffer& out_buf);

int buffer_to_value(const int_buffer& in_buffer);

} //namespace utils
}//namespace nxudp

#endif //NX_UDP_INT_BUFFER_H
