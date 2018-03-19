//
// Created by n8 on 3/16/18.
//

#include <functional>
#include "client.h"

namespace nxudp
{
client::client(asio::io_service& io,
               const std::string& host,
               const std::string& port,
               int timeout) :
    _socket(io, udp::endpoint(udp::v4(), 0)),
    _timeout(timeout)
{
    if(resolve_endpoint(io, host, port))
    {
        send_timeout();
        wait();
    }
}

bool client::resolve_endpoint(asio::io_service& io, const std::string& host, const std::string& port)
{
    udp::resolver resolver(io);
    udp::resolver::query query(udp::v4(), host, port);

    asio::error_code error;
    udp::resolver::iterator it = resolver.resolve(query, error);
    udp::resolver::iterator end;

    if(error)
    {
        std::cout << "Invalid host:port combination provided." << host << ":" << port << std::endl;
        return false;
    }

    if(it == end) // invalid host and port provided.
    {
        std::cout << "Invalid host:port combination provided." << std::endl;
        return  false;
    }
    else
    {
        _remote_endpoint = *it;
        return true;
    }
}

void client::send_timeout()
{
    auto func = std::bind(&client::async_send_callback, this,
                          std::placeholders::_1,
                          std::placeholders::_2);

    nxudp::utils::populate_buffer(_timeout, _send_buffer);

   _socket.async_send_to(asio::buffer(_send_buffer), _remote_endpoint, func);
}

void client::wait()
{
    auto func = std::bind(&client::async_receive_callback, this,
                          std::placeholders::_1,
                          std::placeholders::_2);

    _socket.async_receive_from(asio::buffer(_receive_buffer), _remote_endpoint, func);
}

void client::async_send_callback(const asio::error_code &error, std::size_t bytes_transferred)
{
    if(error)
    {
        std::cout << "Error sending: " << error << std::endl;
    }
    else
    {
        std::cout << "Sent datagram with value \"" << _timeout << "\" to " << _remote_endpoint
                  << ".\nWaiting for response..." << std::endl;
    }
}

void client::async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred)
{
    if(error)
    {
        std::cout << "Error receiving:" << error << std::endl;
    }
    else
    {
        std::string message;
        buffer_to_string(_receive_buffer, bytes_transferred, message);
        std::cout << "Received response \"" << message << "\" from " << _remote_endpoint << "." << std::endl;
    }
}

void client::buffer_to_string(const nxudp::client::receive_buffer &buf, std::size_t bytes_transferred, std::string &out_message)
{
    out_message.reserve(bytes_transferred);

    for(int i = 0; i < bytes_transferred; ++i)
    {
        out_message.push_back(buf[i]);
    }
}


}// namespace nxudp