//
// Created by n8 on 3/16/18.
//

#include <iostream>
#include "server.h"
#include "client_waiter.h"

namespace nxudp
{

server::server(asio::io_service& io) :
    _io(io),
    _socket(io, udp::endpoint(udp::v4(), 0))
{
    unsigned short port = _socket.local_endpoint().port();
    std::cout << "Listening port " << port << std::endl;

    start_receive();
}

void server::start_receive()
{
    auto func = std::bind(&server::async_receive_callback, this,
                          std::placeholders::_1,
                          std::placeholders::_2);

    _socket.async_receive_from(asio::buffer(_receive_buffer), _remote_endpoint, func);
}

void server::async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred)
{
    if(error)
    {
        std::cout << "Error receiving: " << error << std::endl;
        return;
    }

    int timeout;
    if(try_parse_timeout(_receive_buffer, bytes_transferred, timeout))
    {
        auto waiter = std::make_shared<client_waiter>(*this, _io, _remote_endpoint, timeout);

        _waiters.insert(waiter);

        std::cout << "Received request from " << _remote_endpoint << " with value \"" << timeout << "\"" << std::endl;

        start_receive();
    }
    else
        std::cout << "The server received an invalid timeout from client: "
                  << _remote_endpoint << ". Ignoring" << std::endl;
}

    void server::async_send_callback(const std::shared_ptr<client_waiter> &waiter,
                                     const std::string &message,
                                     const asio::error_code &error,
                                     std::size_t bytes_transferred)
    {
        if(error)
            std::cout << error << std::endl;

        std::cout << "Sent response \"" << message << "\" to " << waiter->remote_endpoint() << std::endl;

        _waiters.erase(waiter);
    }

    bool server::try_parse_timeout(server::buffer &buf, size_t transferred, int& timeout)
    {
        try
        {
            std::string s;
            s.reserve(transferred);

            for(int i = 0; i < transferred; ++i)
                s += buf[i];

            timeout = std::stoi(s);
            return true;
        }
        catch(std::exception& e)
        {
            return false;
        }
    }

    void server::wait_completed(const std::shared_ptr<client_waiter>& waiter)
    {
        auto func = std::bind(&server::async_send_callback, this,
                                waiter,
                                _done,
                                std::placeholders::_1,
                                std::placeholders::_2);

        _socket.async_send_to(asio::buffer(_done), waiter->remote_endpoint(), func);
    }

}