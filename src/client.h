//
// Created by n8 on 3/15/18.
//

//
// client.h
// ~~~~~~~~~~
//

#include <iostream>
#include <array>
#include "session_data.h"
#include "int_buffer.h"
#include "session_data.h"
#include "export.h"
#include "network_object.h"

namespace nxudp
{

/// Represents a client when the app is run in client mode. This class sends a timeout, in milliseconds, to the server.
/// The server then waits the specfied amount of time before replying back to the client.
class NXUDP_API client : network_object
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
    client(asio::io_service& io, const asio::ip::udp::endpoint&server_endpoint, int timeout);

    virtual ~client();

private:

    /// connects the _socket to the server_endpoint()
    void connect_to_server();

    /// Calls the socket's async_send_to() function.
    void send_timeout();

    /// Calls the socket's async_receive_from() function;
    void wait();

    /// The callback function called by asio when _socket.async_send_to completes..
    /// @param[in] error - an error code if one occured, provided by asio.
    /// @param[in] bytes_transferred - the number of bytes sent out on the _socket, provided by asio.
    void async_send_callback(const asio::error_code &error, std::size_t bytes_transferred);

    /// The callback function called by asio when _socket.async_receive_from() completes.
    /// @param[in] error - an error code if one occured, provided by asio.
    /// @param[in] bytes_transferred - the number of bytes sent out on the _socket, provided by asio.
    void async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred);

    /// The callback function called by asiuon when _socket.async_connect() completes.
    /// @param[in] error - the error code in one occured.
    void async_connect_callback(const asio::error_code &error);

    /// A helper function that converts the contents of the given buffer to a string.
    /// @param[in] buffer - the buffer whose contents to convert, typically filled in by the socket itself.
    /// @param[in] bytes_transferred - the number of bytes transferred to the socket.
    /// @param[out] out_message - the string that will be assigned the parsed contents of the buffer
    void parse_response(const nxudp::client::receive_buffer &buffer, std::size_t bytes_transferred,
                        std::string& out_message);

    /// get the endpoint of the server this client is connected to.
    const asio::ip::udp::endpoint& server_endpoint() const;

    /// get the time out for this client
    int timeout() const;

private:

    int _timeout;
};

}// namespace nxudp