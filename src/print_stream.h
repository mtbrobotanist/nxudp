//
// Created by n8 on 3/21/18.
//

#ifndef NXUDP_PRINT_STREAM_H
#define NXUDP_PRINT_STREAM_H

#include <sstream>
#include <iostream>
#include "export.h"

namespace nxudp
{

namespace detail {

class NXUDP_API print_stream : public std::ostringstream
{
public:
    print_stream() : _os(std::cout) {}
    print_stream(std::ostream& os) :_os(os) {}

    ~print_stream()
    {
        _os << this->str();
    }

private:
    std::ostream& _os;
};

} // detail

class NXUDP_API stdcout: public detail::print_stream
{
public:
    stdcout(): print_stream(std::cout){ }
};

class NXUDP_API stdcerr: public detail::print_stream
{
public:
    stdcerr(): print_stream(std::cerr){ }
};

}

#endif //NXUDP_PRINT_STREAM_H
