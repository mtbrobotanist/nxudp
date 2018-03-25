//
// Created by n8 on 3/23/18.
//

#ifndef NXUDP_CLIENT_BASE_H
#define NXUDP_CLIENT_BASE_H

#include <asio.hpp>
#include "export.h"

/// This class represents the timer session information in either the client context or the server context
///  - timeout
///  - ip:port information for a remote endpoint.

namespace nxudp
{
struct NXUDP_API session_data
{
    session_data(const asio::ip::udp::endpoint &remote_endpoint, int timeout):
            remote_endpoint(remote_endpoint),
            timeout(timeout)
    { }

    session_data(const session_data& other) :
            timeout(other.timeout),
            remote_endpoint(other.remote_endpoint)
    { }

    /// The remote endpoint of either the client or the server, depending on where the _data was instantiated.
    asio::ip::udp::endpoint remote_endpoint;

    /// The timeout, in milliseconds
    int timeout;
};
}


#endif //NXUDP_CLIENT_BASE_H
