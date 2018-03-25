//
// Created by n8 on 3/24/18.
//

#include "network_object.h"


namespace nxudp
{

    using asio::ip::udp;

    network_object::network_object(asio::io_service& io) :
        _socket(io, udp::endpoint(udp::v4(), 0))
    { }

    network_object::network_object(asio::io_service& io, const udp::endpoint& remote_endpoint) :
            _socket(io, udp::endpoint(udp::v4(), 0)),
            _remote_endpoint(remote_endpoint)
    { }
}