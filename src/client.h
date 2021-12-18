//
// Created by n8 on 3/15/18.
//

//
// client.h
// ~~~~~~~~~~
//

#include <iostream>
#include <array>
#include "asio/io_context.hpp"
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
    /// @param[in] io - the asio::io_context object requried to run the internal asio::socket.
    /// @param[in] host - the host to connect to, e.g. "localhost".
    /// @param[in] host - the port to connect to, e.g. "12345".
    /// @param[in] timeout - the amount of time, in milliseconds, the server should wait before sending a response.
    /// This is the timeout sent out on the socket to the server.
    client(asio::io_context& io, const std::string&host, const std::string& port, int timeout);

    ~client();

private:
    void resolve(const std::string& host, const std::string& port);
    asio::ip::udp::resolver::results_type dropIpv6(const asio::ip::udp::resolver::results_type& resolved_endpoints);


    /// connects the _socket to the server_endpoint()
    void connect_to_server(asio::ip::udp::resolver::results_type resolved_endpoints);

    /// Calls the socket's async_send_to() function.
    void send_timeout();

    /// Calls the socket's async_receive_from() function;
    void wait();

    /// get the time out for this client
    int timeout() const;

private:
    asio::ip::udp::resolver _resolver;
    asio::ip::udp::socket _socket;
    const int _timeout;

    int_buffer _timeout_buffer; //< for sending the integer to the server.
    std::string _response_buffer; //< for receiving the response.
};

}// namespace nxudp