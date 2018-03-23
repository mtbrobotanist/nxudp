//
// Created by n8 on 3/18/18.
//

#ifndef NX_UDP_INT_BUFFER_H
#define NX_UDP_INT_BUFFER_H

#include <array>
#include "export.h"

/// Some utility functions for converting from an integer to a asoi::buffer compatible buffer and back.

namespace nxudp
{

typedef std::array<uint8_t, sizeof(int)> int_buffer;

namespace utils
{

/// Write the given integer to the given buffer.
/// @param[in] value - the integer to convert.
/// @param[out] out_buffer - the int_buffer to populate.
void NXUDP_API write_buffer(int value, int_buffer &out_buffer);


/// Convert the given int_buffer to an integer.
/// @param[in] in_buffer - the buffer whose contents to parse to an integer.
/// @returns int - the integer converted from the buffer.
int NXUDP_API read_buffer(const int_buffer &in_buffer);

} //namespace utils
}//namespace nxudp

#endif //NX_UDP_INT_BUFFER_H
