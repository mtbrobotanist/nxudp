//
// Created by n8 on 3/16/18.
//

#include <iostream>
#include "client_waiter.h"
#include "print_stream.h"

namespace nxudp
{

using asio::ip::udp;

client_waiter::client_waiter(nxudp::server& server, asio::io_service& io, const client_info& client_info) :
    _client_info(client_info),
    _server(server),
    _timer(io, std::chrono::milliseconds(_client_info.timeout()))
{ }

client_waiter::~client_waiter()
{ }

void client_waiter::async_timer_callback(const asio::error_code& error)
{
    if(error)
    {
        print_stream(std::cerr) << "Error during timed wait: " << error << std::endl;
    }

    _server.wait_completed(shared_from_this());
}

void client_waiter::start_wait()
{
    auto func =
            std::bind(&client_waiter::async_timer_callback, this, std::placeholders::_1);

    _timer.async_wait(std::bind(&client_waiter::async_timer_callback, this, std::placeholders::_1));
}

const asio::ip::udp::endpoint &client_waiter::client_endpoint() const
{
    return _client_info.remote_endpoint();
}

}// namespace nxudp