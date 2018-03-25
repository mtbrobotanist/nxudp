//
// Created by n8 on 3/24/18.
//

#ifndef NXUDP_NETWORK_OBJECT_H
#define NXUDP_NETWORK_OBJECT_H


#include <asio.hpp>
#include "int_buffer.h"
#include "export.h"


namespace nxudp
{

class NXUDP_API network_object
{

public:
    network_object(asio::io_service& io);
    network_object(asio::io_service& io, const asio::ip::udp::endpoint& remote_endpoint);

protected:
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _remote_endpoint;

    int_buffer _timeout_buffer;
};

}



#endif //NXUDP_NETWORK_OBJECT_H
