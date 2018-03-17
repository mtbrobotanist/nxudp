//+
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_SERVER_H
#define NX_UDP_SERVER_H

#include <map>
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <set>

namespace nxudp
{

using asio::ip::udp;


class client_waiter;


class server
{

    typedef std::array<char, 128> buffer;

public:
    server(asio::io_service& io);

    void wait_completed(const std::shared_ptr<client_waiter> &waiter);

private:

    void async_send_callback(const std::shared_ptr<client_waiter> &waiter,
                             const std::string &message,
                             const asio::error_code &error,
                             std::size_t bytes_transferred);

    bool try_parse_timeout(buffer &vector, size_t transferred, int& timeout);

    void start_receive();

    void async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred);


private:
    asio::io_service& _io;
    udp::socket _socket;
    udp::endpoint _remote_endpoint;
    buffer _receive_buffer;

    std::set<std::shared_ptr<client_waiter>> _waiters;

    const std::string _done = "DONE";
};

}

#endif //NX_UDP_SERVER_H
