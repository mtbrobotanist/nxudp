//
// Created by n8 on 3/15/18.
//

//
// client.h
// ~~~~~~~~~~
//

#include <iostream>
#include <array>
#include "asio/io_service.hpp"
#include "asio/ip/udp.hpp"
#include "int_buffer.h"
#include "export.h"

namespace nxudp
{

/// Represents a client when the app is run in client mode. This class sends a timeout, in milliseconds, to the server.
/// The server then waits the specfied amount of time before replying back to the client.
class NXUDP_API client
{

public:

    typedef int_buffer send_buffer;

public:
    /// Constructs a client that will send the timeout.
    /// @param[in] io - the asio::io_service object requried to run the internal asio::socket.
    /// @param[in] server_endpoint - the host port combination to send the timeout to.
    /// @param[in] timeout - the amount of time, in milliseconds, the server should wait before sending a response.
    /// This is the timeout sent out on the socket to the server.
    client(asio::io_service& io, const asio::ip::udp::endpoint&server_endpoint, int timeout);

    ~client();

private:
    /// connects the _socket to the server_endpoint()
    void connect_to_server();

    /// Calls the socket's async_send_to() function.
    void send_timeout();

    /// Calls the socket's async_receive_from() function;
    void wait();

    /// get the time out for this client
    int timeout() const;

private:
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _server_endpoint;
    int _timeout;

    int_buffer _timeout_buffer;
    std::string _message_buffer;
};

}// namespace nxudp