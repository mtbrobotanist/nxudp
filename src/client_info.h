//
// Created by n8 on 3/23/18.
//

#ifndef NXUDP_CLIENT_BASE_H
#define NXUDP_CLIENT_BASE_H

#include <asio.hpp>
#include "export.h"

namespace nxudp
{
class NXUDP_API client_info
{

public:
    client_info(const asio::ip::udp::endpoint& remote_endpoint, int timeout);

    client_info(const client_info& other);

    int timeout() const;

    const asio::ip::udp::endpoint& remote_endpoint() const;

private:
    asio::ip::udp::endpoint _remote_endpoint;
    int _timeout;
};
}


#endif //NXUDP_CLIENT_BASE_H
