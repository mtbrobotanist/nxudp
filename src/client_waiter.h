//
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_CLIENTINFO_H
#define NX_UDP_CLIENTINFO_H

#include <asio.hpp>
#include "server.h"

namespace nxudp
{

using asio::ip::udp;

class client_waiter : public std::enable_shared_from_this<client_waiter>
{
public:
    client_waiter(server& server,
                     asio::io_service& io,
                     const udp::endpoint& remote_endpoint,
                     int timeout);

    const udp::endpoint& remote_endpoint() const { return _remote_endpoint; }

private:
    void async_timer_callback(const asio::error_code& error);

    void start_wait();

private:
    server& _server;
    udp::endpoint _remote_endpoint;
    asio::steady_timer _timer;
    int _timeout;
};

}

#endif //NX_UDP_CLIENTINFO_H
