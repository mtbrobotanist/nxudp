//
// Created by n8 on 3/15/18.
//

//
// client.h
// ~~~~~~~~~~
//

#include <iostream>
#include <array>
#include "client_info.h"
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
    typedef std::array<char , 128> receive_buffer;

public:
    /// Constructs a client that will send the timeout.
    /// @param[in] io - the asio::io_service object requried to run the internal asio::socket.
    /// @param[in] server_endpoint - the host port combination to send the timeout to.
    /// @param[in] timeout - the amount of time, in milliseconds, the server should wait before sending a response.
    /// This is the timeout sent out on the socket to the server.
    client(asio::io_service& io, const client_info& client_info);

    virtual ~client();

private:

    /// get the endpoint of the server this client is connected to.
    const asio::ip::udp::endpoint& server_endpoint() const;

    int timeout() const;

    /// Calls the socket's async_send_to() function.
    void send_timeout();

    /// Calls the socket's async_receive_from() function;
    void wait();

    /// The callback function called by asio when _socket.async_send_to completes..
    /// @param[in] error - an error code if one occured, provided by asio.
    /// @param[in] bytes_transferred - the number of bytes sent out on the _socket, provided by asio.
    void async_send_callback(const asio::error_code &error, std::size_t bytes_transferred);

    /// The callback function called by asio when _socket.async_receive_from completes.
    /// @param[in] error - an error code if one occured, provided by asio.
    /// @param[in] bytes_transferred - the number of bytes sent out on the _socket, provided by asio.
    void async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred);


    void async_connect_callback(const asio::error_code &error);
private:

    client_info _client_info;

    /// The socket used to send the timeout to the server.
    asio::ip::udp::socket _socket;

    /// The buffer that our timeout will be written to.
    /// This buffer is written to the socket.
    send_buffer _send_buffer;

    /// The buffer that received the response from the server.
    /// The socket writes to the this buffer, we read from it.
    receive_buffer _receive_buffer;

    void connect_to_server();
};

}// namespace nxudp