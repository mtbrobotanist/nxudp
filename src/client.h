//
// Created by n8 on 3/15/18.
//

//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.std.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <array>
#include <asio.hpp>
#include "int_buffer.h"

using asio::ip::udp;

namespace nxudp
{

class client
{

    typedef int_buffer send_buffer;
    typedef std::array<char , 128> receive_buffer;

public:
    client(asio::io_service& io, const std::string& host, const std::string& port, int timeout);

    void async_send_callback(const asio::error_code &error, std::size_t bytes_transferred);

    void async_receive_callback(const asio::error_code &e, std::size_t bytes_transferred);

private:
    bool resolve_endpoint(asio::io_service& io, const std::string& host, const std::string& port);
    void send_timeout();
    void wait();
    void buffer_to_string(nxudp::client::receive_buffer &buf, std::size_t bytes_transferred, std::string &out_message);

private:
    udp::socket _socket;
    udp::endpoint _remote_endpoint;
    int _timeout;

    send_buffer _send_buffer;
    receive_buffer _receive_buffer;

};

}// namespace nxudp