//
// Created by n8 on 3/16/18.
//

#include "server.h"
#include "asio/error_code.hpp"
#include "asio/ip/udp.hpp"
#include "asio/steady_timer.hpp"
#include "print_stream.h"
#include <memory>

namespace nxudp
{

using asio::ip::udp;

server::server(asio::io_service& io) :
    _io(io),
    _socket(io, udp::endpoint(udp::v4(), 0))
{
    unsigned short port = _socket.local_endpoint().port();
    print_stream() << "Listening port " << port << std::endl;

    start_receive();
}

server::~server()
{ }

void server::start_receive()
{
    auto session = std::make_unique<client_session>();
    auto session_ptr = session.get();
    _socket.async_receive_from(
        asio::buffer(session_ptr->timeout_buffer),
        session_ptr->client_endpoint,
        [this, session = std::move(session)](const asio::error_code &error, std::size_t bytes_transferred) mutable
        {
            if (error)
            {
                print_stream(std::cerr) << "Got an error while waiting for client connection" << error << std::endl;
                // we want to continue listening for other clients even if there was an error
                return start_receive();
            }

            int timeout;
            if (!parse_timeout(session->timeout_buffer, bytes_transferred, timeout))
            {
                print_stream(std::cerr) << "Got an invalid timeout: " << session->timeout_buffer.data() << std::endl;
                return start_receive();
            }

            session->timer = std::make_unique<asio::steady_timer>(_io, std::chrono::milliseconds(timeout));
            print_stream() << "Received request from " << session->client_endpoint << " with value \"" << timeout << "\"" << std::endl;
            start_session(std::move(session));

            start_receive();
        });
}

bool server::parse_timeout(int_buffer &buffer, size_t bytes_transferred, int &out_timeout)
{
    if(bytes_transferred < sizeof(int))
    {
        out_timeout = std::numeric_limits<int>::min();
        return false;
    }

    out_timeout = nxudp::utils::read_buffer(buffer);
    return true;
}

void server::end_session(client_session *session)
{
    _socket.async_send_to(
        asio::buffer(_RESPONSE),
        session->client_endpoint,
        [this, session](const asio::error_code& error, std::size_t)
        {
            if (error)
                print_stream(std::cerr) << "Error while sending response to client " << session->client_endpoint << std::endl;
            else
                print_stream() << "Sent response \"" << _RESPONSE << "\" to " << session->client_endpoint << std::endl;

            _sessions.erase(session);
        });
}

void server::start_session(std::unique_ptr<client_session> session)
{
    auto session_ptr = session.get();
    _sessions.emplace(session_ptr, std::move(session));

    session_ptr->timer->async_wait(
        [this, session_ptr](const asio::error_code& error)
        {
            if (error)
                print_stream(std::cerr) << "Error during timed wait: " << error << "\n";

            end_session(session_ptr);
        });
}

}// namespace nxudp