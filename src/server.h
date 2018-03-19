//+
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_SERVER_H
#define NX_UDP_SERVER_H

#include <unordered_set>
#include <asio.hpp>
#include "int_buffer.h"

namespace nxudp
{

using asio::ip::udp;


class client_waiter;


class server
{

    typedef int_buffer receive_buffer;

    const std::string _RESPONSE = "DONE";

public:
    server(asio::io_service& io);

    void wait_completed(const std::shared_ptr<client_waiter> &waiter);

private:

    void async_send_callback(const std::shared_ptr<client_waiter> &waiter,
                             const std::string &message,
                             const asio::error_code &error,
                             std::size_t bytes_transferred);

    bool
    try_parse_timeout(server::receive_buffer &buffer, size_t bytes_transferred, int &out_timeout);

    void start_receive();

    void async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred);


private:
    asio::io_service& _io;
    udp::socket _socket;
    udp::endpoint _remote_endpoint;
    receive_buffer _receive_buffer;

    std::unordered_set<std::shared_ptr<client_waiter>> _waiters;
};

}// namespace nxudp

#endif //NX_UDP_SERVER_H
