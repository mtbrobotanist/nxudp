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
#include "export.h"

using asio::ip::udp;

namespace nxudp
{

/// Represents a client when the app is run in client mode. This class sends a timeout, in milliseconds, to the server.
/// The server then waits the specfied amount of time before replying back to the client.
class NXUDP_API client
{

    typedef int_buffer send_buffer;
    typedef std::array<char , 128> receive_buffer;

public:
    /// Constructs a client to that will send the timeout.
    /// @param[in] io - the asio::io_service object requried to run the internal asio::socket.
    /// @param[in] host - the host to send timeout to.
    /// @param[in] port - the host's port to send the timeout to.
    /// @param[in] timeout - the amount of time, in milliseconds, the server should wait before sending a response.
    /// This is the timeout sent out on the socket to the server.
    client(asio::io_service& io, const std::string& host, const std::string& port, int timeout);
    
    /// Constructs a client to that will send the timeout.
    /// @param[in] io - the asio::io_service object requried to run the internal asio::socket.
    /// @param[in] host - the host to send timeout to.
    /// @param[in] port - the host's port to send the timeout to.
    /// @param[in] timeout - the amount of time, in milliseconds, the server should wait before sending a response, represented as a string.
    /// This is the timeout sent out on the socket to the server.
    client(asio::io_service& io, const std::string& host, const std::string& port, const std::string& timeout);

    virtual ~client();

    /// The callback function called by asio when _socket.async_send_to completes..
    /// @param[in] error - an error code if one occured, provided by asio.
    /// @param[in] bytes_transferred - the number of bytes sent out on the _socket, provided by asio.
    void async_send_callback(const asio::error_code &error, std::size_t bytes_transferred);

    /// The callback function called by asio when _socket.async_receive_from completes.
    /// @param[in] error - an error code if one occured, provided by asio.
    /// @param[in] bytes_transferred - the number of bytes sent out on the _socket, provided by asio.
    void async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred);

private:
    
    /// A helper function that parses a string to an integer, assigning it to @param out_int if successful.
    /// returns true if @param out_timeout was assigned, false otherwise.
    /// @param[in] int_string - a string representation of an integer
    /// @param[out] out_int - a reference to an int that will be assigned the pared timeout
    bool parse_int(const std::string& int_string, int& out_int) const;
    
    /// Calls the socket's async_send_to() function.
    void send_timeout();

    /// Calls the socket's async_receive_from() function;
    void wait();

    /// A helper function that converts the contents of the given buffer to a string.
    /// @param[in] buffer - the buffer whose contents to convert, typicall filled in by the socket itself.
    /// @param[in] bytes_transferred - the number of bytes transferred to the socket.
    /// @param[out] out_message - the string that will be assigned the parsed contents of the buffer
    void buffer_to_string(const nxudp::client::receive_buffer &buffer, std::size_t bytes_transferred, std::string &out_message);
    
private:
    
    /// The socket used to send the timeout to the server.
    udp::socket _socket;

    /// The remote endpoint of the server.
    udp::endpoint _remote_endpoint;

    /// The timeout to send to the server.
    int _timeout;

    /// The buffer that our timeout will be written to.
    /// This buffer is written to the socket.
    send_buffer _send_buffer;

    /// The buffer that received the response from the server.
    /// The socket writes to the this buffer, we read from it.
    receive_buffer _receive_buffer;

};

}// namespace nxudp