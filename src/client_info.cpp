//
// Created by n8 on 3/23/18.
//

#include "client_info.h"

namespace nxudp
{

client_info::client_info(const asio::ip::udp::endpoint &remote_endpoint, int timeout):
    _remote_endpoint(remote_endpoint),
    _timeout(timeout)
{}

client_info::client_info(const client_info& other) :
    _timeout(other._timeout),
    _remote_endpoint(other._remote_endpoint)
{}

int client_info::timeout() const
{
    return _timeout;
}

const asio::ip::udp::endpoint& client_info::remote_endpoint() const
{
    return _remote_endpoint;
}

}