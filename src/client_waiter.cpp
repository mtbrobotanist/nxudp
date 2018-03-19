//
// Created by n8 on 3/16/18.
//

#include <iostream>
#include "client_waiter.h"

namespace nxudp
{

client_waiter::client_waiter(nxudp::server& server,
                               asio::io_service& io,
                               const udp::endpoint& remote_endpoint,
                               int timeout) :
    _server(server),
    _remote_endpoint(remote_endpoint),
    _timer(io, std::chrono::milliseconds(timeout))
{
}

void client_waiter::async_timer_callback(const asio::error_code& error)
{
    if(error)
        std::cout << "Error during timed wait: " << error << std::endl;

    _server.wait_completed(shared_from_this());
}

void client_waiter::start_wait()
{
    auto func =
            std::bind(&client_waiter::async_timer_callback, this, std::placeholders::_1);
    _timer.async_wait(func);
}

}// namespace nxudp