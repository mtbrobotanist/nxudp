//
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_CLIENTINFO_H
#define NX_UDP_CLIENTINFO_H

#include "server.h"
#include "export.h"
#include "session_data.h"

namespace nxudp
{

/// Represents and implements the wait period before the server sends a reply back to the client.
class NXUDP_API timed_session : public std::enable_shared_from_this<timed_session>
{

public:

    typedef std::shared_ptr<timed_session> ptr;

    /// Constructs a client_waiter object.
    /// @param[in] io - the asio::io_service required to run the client_waiter timer.
    /// @param[in] remote_endpoint - the endpoint of the client that this client_waiter represents.
    /// @param timeout - the amount of time, in milliseconds that this client_waiter should wait before triggering the server's response to the client.
    timed_session(server& server, asio::io_service& io, const session_data& client_data);

	virtual ~timed_session();

    /// Get the remote endpoint that this client_waiter represents.
    /// @returns a const reference to this client_waiters udp::remote endpoint object.
    const asio::ip::udp::endpoint& client_endpoint() const;

    /// Starts the count down to trigger a response. Called by the owning server object.
    void start_wait();

private:

    /// The callback used by _timer.async_wait().
    /// Triggers the server to send a reponse.
    /// @param[in]error - an error code, if one occured
    void async_timer_callback(const asio::error_code& error);

private:
    /// The client data, timeout and remote endpoint.
    /// In the context of client waiter, _client_data.remote_endpoint represents the clients ip:port combo
    session_data _client_data;

    /// The server that owns this client waiter.
    server& _server;

    /// the timer used to countdown to a response.
    asio::steady_timer _timer;
};

}// namespace nxudp

#endif //NX_UDP_CLIENTINFO_H
