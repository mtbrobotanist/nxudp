//
// Created by n8 on 3/21/18.
//

#ifndef NXUDP_PRINT_STREAM_H
#define NXUDP_PRINT_STREAM_H

#include <sstream>
#include <mutex>
#include <iostream>
#include "export.h"

namespace nxudp
{

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

}

#endif //NXUDP_PRINT_STREAM_H
