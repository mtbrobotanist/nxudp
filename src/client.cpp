//
// Created by n8 on 3/16/18.
//

#include <functional>
#include "client.h"
#include "endpoint_utils.h"
#include "print_stream.h"

namespace nxudp
{

using asio::ip::udp;

client::client(asio::io_service& io, const asio::ip::udp::endpoint& server_endpoint, int timeout) :
    _socket(io, udp::endpoint(udp::v4(), 0)),
    _server_endpoint(server_endpoint),
    _timeout(timeout),
    _message_buffer(4, 0) //< "DONE"
{
    connect_to_server();
}

client::~client()
{
    stdcout() << "Exiting..." << std::endl;
}

void client::connect_to_server()
{
    _socket.async_connect(
        _server_endpoint,
        [this](const asio::error_code &error)
        {
            if(error)
                stdcerr() << "Error connecting to server: " << error << "\n";
            else
                send_timeout();
        });
}

void client::send_timeout()
{
    nxudp::utils::write_buffer(_timeout, _timeout_buffer);

    _socket.async_send(
        asio::buffer(_timeout_buffer),
        [this](const asio::error_code &error, std::size_t)
        {
            if(error)
            {
                stdcerr() << "Error sending: " << error << "\n";
            }
            else
            {
                stdcout() << "Sent datagram with value \"" << _timeout << "\" to " << _server_endpoint
                        << ".\nWaiting for response...\n";

                wait();
            }
        });
}

void client::wait()
{
    _socket.async_receive(
        asio::buffer(_message_buffer),
        [this](const asio::error_code &error, std::size_t bytes_transferred)
        {
            if (error)
                stdcerr() << "Error receiving:" << error << "\n";
            else
                stdcout() << "Received response \"" << _message_buffer << "\" from " << _server_endpoint << ".\n";
        });
}

}// namespace nxudp