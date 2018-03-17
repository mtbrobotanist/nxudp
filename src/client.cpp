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
                   const std::string& timeout) :
        _socket(io, udp::endpoint(udp::v4(), 0)),
        _timeout(timeout)
    {
        udp::resolver resolver(io);
        udp::resolver::query query(udp::v4(), host, port);

        asio::error_code error;
        udp::resolver::iterator it = resolver.resolve(query, error);
        udp::resolver::iterator end;

        if(error)
        {
            std::cout << "Invalid host:port combination provided." << host << ":" << port << std::endl;
            return;
        }

        if(it == end) // invalid host and port provided.
        {
            std::cout << "Invalid host:port combination provided." << std::endl;
        }
        else
        {
            _remote_endpoint = *it;

            send_timeout();

            wait();
        }
    }

    void client::send_timeout()
    {
        auto func = std::bind(&client::async_send_callback, this,
                              std::placeholders::_1,
                              std::placeholders::_2);

       _socket.async_send_to(asio::buffer(_timeout), _remote_endpoint, func);
    }

    void client::wait()
    {
        auto func = std::bind(&client::async_receive_callback, this,
                              std::placeholders::_1,
                              std::placeholders::_2);

        _socket.async_receive_from(asio::buffer(_receive_buf), _remote_endpoint, func);
    }

    void client::async_send_callback(const asio::error_code &error, std::size_t bytes_transferred)
    {
        if(error)
        {
            std::cout << error << std::endl;
        }
        else
        {
            std::cout << "Sent datagram with value \"" << _timeout << "\" to " << _remote_endpoint
                      << ".\nWaiting for response..." << std::endl;
        }
    }

    void client::async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred)
    {
        if(!error)
        {
            std::string message = buffer_to_string(_receive_buf, bytes_transferred);
            std::cout << "Received response \"" << message << "\" from " << _remote_endpoint << "." << std::endl;
        }
        else
        {
            std::cout << error << std::endl;
        }
    }

    std::string client::buffer_to_string(nxudp::client::buffer &buf, std::size_t len)
    {
        std::string s;
        s.reserve(len);

        for(int i = 0; i < len; ++i)
         s += buf[i];

        return s;
    }
}