//
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_CLIENTINFO_H
#define NX_UDP_CLIENTINFO_H

#include "server.h"
#include "export.h"
#include "client_info.h"

namespace nxudp
{

/// Represents and implements the wait period before the server sends a reply back to the client.
class NXUDP_API client_waiter : public std::enable_shared_from_this<client_waiter>
{

public:

    typedef std::shared_ptr<client_waiter> ptr;

    /// Constructs a client_waiter object.
    /// @param[in] io - the asio::io_service required to run the client_waiter timer.
    /// @param[in] remote_endpoint - the endpoint of the client that this client_waiter represents.
    /// @param timeout - the amount of time, in milliseconds that this client_waiter should wait before triggering the server's response to the client.
    client_waiter(server& server, asio::io_service& io, const client_info& client_info);

	virtual ~client_waiter();

    /// Get the remote endpoint that this client_waiter represents.
    /// @returns a const reference to this client_waiters udp::remote endpoint object.
    const asio::ip::udp::endpoint& client_endpoint() const;

    /// Starts the count down to trigger a response. Called by the owning server object.
    void start_wait();

private:

    /// The callback used by _timer.async_wait().
    /// Triggers the server to send a reponse.
    void async_timer_callback(const asio::error_code& error);

private:

    client_info _client_info;

    /// The server that owns this client waiter.
    server& _server;

    /// the timer used to countdown to a response.
    asio::steady_timer _timer;
};

}// namespace nxudp

#endif //NX_UDP_CLIENTINFO_H
