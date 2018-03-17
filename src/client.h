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

using asio::ip::udp;

namespace nxudp
{

class client
{
public:
    client(asio::io_service& io, const std::string& host, const std::string& port, const::std::string& timeout);

    void async_send_callback(const asio::error_code &error, std::size_t bytes_transferred);

    void async_receive_callback(const asio::error_code &e, std::size_t bytes_transferred);

private:
    typedef std::array<char, 128> buffer;

    void send_timeout();
    void wait();
    std::string buffer_to_string(buffer &buf, std::size_t len);

private:

    udp::socket _socket;
    udp::endpoint _remote_endpoint;
    std::string _timeout;

    buffer _receive_buf;
};

}