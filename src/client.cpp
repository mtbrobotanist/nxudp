//
// Created by n8 on 3/16/18.
//

#include <algorithm>
#include <functional>
#include <iterator>
#include "asio/connect.hpp"
#include "asio/error_code.hpp"
#include "asio/ip/udp.hpp"
#include "client.h"
#include "print_stream.h"

namespace nxudp
{

using asio::ip::udp;

client::client(asio::io_context& io, const std::string& host, const std::string& port, int timeout) :
    _resolver(io),
    _socket(io, udp::endpoint(udp::v4(), 0)),
    _timeout(timeout),
    _response_buffer(4, 0) //< "DONE"
{
    resolve(host, port);
}

client::~client()
{
    stdcout() << "Exiting...";
}

void client::resolve(const std::string& host, const std::string& port)
{
    _resolver.async_resolve(
        host,
        port,
        udp::resolver::flags::numeric_service,
        [this](const asio::error_code& error, udp::resolver::results_type results)
        {
            if (error)
            {
                stdcerr() << "Server address resolution failed: " << error;
                return;
            }

            connect_to_server(dropIpv6(results));
        });
}

udp::resolver::results_type client::dropIpv6(const udp::resolver::results_type& resolved_endpoints)
{
    std::list<udp::endpoint> ipv4_list;
    for(const auto& result: resolved_endpoints)
    {
        if(result.endpoint().protocol() == udp::v4())
            ipv4_list.emplace_back(result.endpoint());
    }

    auto begin = resolved_endpoints.begin();
    return udp::resolver::results_type::create(ipv4_list.begin(), ipv4_list.end(), begin->host_name(), begin->service_name());
}

void client::connect_to_server(udp::resolver::results_type resolved_endpoints)
{
    asio::async_connect(
        _socket,
        resolved_endpoints,
        [this](const asio::error_code& error, const udp::endpoint& endpoint)
        {
            if (error)
            {
                stdcerr() << "Failed to connect to server!";
                return;
            }

            stdcout() << "Connected to server: " << endpoint;

            send_timeout();
        });
}

void client::send_timeout()
{
    nxudp::utils::write_buffer(_timeout, _timeout_buffer);

    _socket.async_send(
        asio::buffer(_timeout_buffer),
        [this](const asio::error_code &error, std::size_t)
        {
            if(error)
            {
                stdcerr() << "Error sending: " << error;
                return;
            }

            stdcout() << "Sent datagram with value \"" << _timeout << "\" to " << _socket.remote_endpoint()
                    << ".\nWaiting for response...";

            wait();
        });
}

void client::wait()
{
    _socket.async_receive(
        asio::buffer(_response_buffer),
        [this](const asio::error_code &error, std::size_t)
        {
            if (error)
                stdcerr() << "Error receiving: " << error;
            else
                stdcout() << "Received response \"" << _response_buffer << "\" from " << _socket.remote_endpoint() << ".";
        });
}

}// namespace nxudp