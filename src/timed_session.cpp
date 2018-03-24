//
// Created by n8 on 3/16/18.
//

#include <iostream>
#include "timed_session.h"
#include "print_stream.h"

namespace nxudp
{

using asio::ip::udp;

timed_session::timed_session(nxudp::server& server, asio::io_service& io, const session_data& client_data) :
    _client_data(client_data),
    _server(server),
    _timer(io, std::chrono::milliseconds(_client_data.timeout))
{ }

timed_session::~timed_session()
{ }

void timed_session::async_timer_callback(const asio::error_code& error)
{
    if(error)
    {
        print_stream(std::cerr) << "Error during timed wait: " << error << "\n";
    }

    _server.end_session(shared_from_this());
}

void timed_session::start()
{
    _timer.async_wait(
            std::bind(&timed_session::async_timer_callback, this, std::placeholders::_1));
}

const asio::ip::udp::endpoint &timed_session::client_endpoint() const
{
    return _client_data.remote_endpoint;
}

}// namespace nxudp