//
// Created by n8 on 3/16/18.
//

#include <functional>
#include "client.h"
#include "endpoint_utils.h"
#include "print_stream.h"

namespace nxudp
{

using asio::ip::udp;

/// A helper function that converts the contents of the given buffer to a string.
/// @param[in] buffer - the buffer whose contents to convert, typically filled in by the socket itself.
/// @param[in] bytes_transferred - the number of bytes transferred to the socket.
/// @param[out] out_message - the string that will be assigned the parsed contents of the buffer
void parse_response(const nxudp::client::receive_buffer &buffer, std::size_t bytes_transferred,
                    std::string &out_message)
{
   out_message.reserve(bytes_transferred);

   for(std::size_t i = 0; i < bytes_transferred; ++i)
   {
       out_message.push_back(buffer[i]);
   }
}


client::client(asio::io_service& io,
               const udp::endpoint& remote_endpoint,
               int timeout) :
    _socket(io, udp::endpoint(udp::v4(), 0)),
    _remote_endpoint(remote_endpoint),
    _timeout(timeout)
{
    send_timeout();
    wait();
}


client::~client()
{
}

void client::send_timeout()
{
    auto func = std::bind(&client::async_send_callback, this,
                          std::placeholders::_1,
                          std::placeholders::_2);

    nxudp::utils::write_buffer(_timeout, _send_buffer);

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
        print_stream(std::cerr) << "Error sending: " << error << "\n";
    }
    else
    {
        print_stream() << "Sent datagram with value \"" << _timeout << "\" to " << _remote_endpoint
                  << ".\nWaiting for response...\n";
    }
}

void client::async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred)
{
    if(error)
    {
        print_stream(std::cerr) << "Error receiving:" << error << "\n";
    }
    else
    {
        std::string message;
        parse_response(_receive_buffer, bytes_transferred, message);
        print_stream() << "Received response \"" << message << "\" from " << _remote_endpoint << ".\n";
    }
}


}// namespace nxudp