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

client::client(asio::io_service& io, const session_data& server_session_data) :
    _session_data(server_session_data),
    _socket(io, udp::endpoint(udp::v4(), 0))
{
    connect_to_server();
}


client::~client()
{
    print_stream() << "Exiting..." << std::endl;
}

void client::connect_to_server()
{
    auto func = std::bind(&client::async_connect_callback, this, std::placeholders::_1);

    _socket.async_connect(server_endpoint(), func);
}

void client::send_timeout()
{
    auto func = std::bind(&client::async_send_callback, this,
                          std::placeholders::_1,
                          std::placeholders::_2);

    nxudp::utils::write_buffer(timeout(), _send_buffer);

   _socket.async_send(asio::buffer(_send_buffer), func);
}

void client::wait()
{
    auto func = std::bind(&client::async_receive_callback, this,
                          std::placeholders::_1,
                          std::placeholders::_2);

    _socket.async_receive(asio::buffer(_receive_buffer), func);
}

void client::async_connect_callback(const asio::error_code &error)
{
    if(error)
    {
        print_stream(std::cerr) << "Error connecting to server: " << error << "\n";
    }
    else
    {
        send_timeout();
    }
}

void client::async_send_callback(const asio::error_code &error, std::size_t)
{
    if(error)
    {
        print_stream(std::cerr) << "Error sending: " << error << "\n";
    }
    else
    {
        print_stream() << "Sent datagram with value \"" << timeout() << "\" to " << server_endpoint()
                  << ".\nWaiting for response...\n";

        wait();
    }
}

void client::async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (error)
    {
        print_stream(std::cerr) << "Error receiving:" << error << "\n";
    }
    else
    {
        std::string message;
        parse_response(_receive_buffer, bytes_transferred, message);

        print_stream() << "Received response \"" << message << "\" from " << server_endpoint() << ".\n";
    }
}

void client::parse_response(const nxudp::client::receive_buffer &buffer,
                            std::size_t bytes_transferred,
                            std::string& out_message)
{
    out_message.reserve(bytes_transferred);

    for(std::size_t i = 0; i < bytes_transferred; ++i)
    {
        out_message.push_back(buffer[i]);
    }
}

const asio::ip::udp::endpoint& client::server_endpoint() const
{
    return _session_data.remote_endpoint;
}

int client::timeout() const
{
    return _session_data.timeout;
}

}// namespace nxudp