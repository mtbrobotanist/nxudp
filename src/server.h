//+
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_SERVER_H
#define NX_UDP_SERVER_H

#include <chrono>
#include <unordered_map>
#include <asio.hpp>
#include "int_buffer.h"
#include "export.h"

namespace nxudp
{
    /// This class is responsible for waiting for input on a socket, parsing the timeout, and constructing timed sessions.
    /// when a timed session expires, it sends a reponse back to the client that initiated the timed session.
class NXUDP_API server
{
    const std::string _RESPONSE = "DONE";

public:
    /// The Constructor for the server object.
    /// @param[in] io - the asio::io_context object required to run the server's socket.
	server(asio::io_context& io);

	virtual ~server();

private:
    struct client_session
    {
        /// The endpoint of the client that started the session. Written to by socket.async_recieve_from().
        asio::ip::udp::endpoint client_endpoint;

        /// The buffer written to by socket.async_receive_from().
        int_buffer timeout_buffer;

        /// The timer used to countdown to a response.
        std::unique_ptr<asio::steady_timer> timer;
    };

    /// A helper function to parse the given buffer to a integer timeout value.
    /// @param[in] buffer - the buffer whose contents to convert to an integer.
    /// @param[in] size_t bytes_transferred, the number of bytes transferred transferred into @param buffer.
    /// @param[out] out_timeout - a reference to an integer that will contain the timeout value.
    /// @returns bool - the result of the conversion. true, if successful, false otherwise.
    bool parse_timeout(int_buffer &buffer, size_t bytes_transferred, int &out_timeout);

    /// The function that calls async_receive_from() on the socket, specifying async_receive_callback() as the future function.
    void start_receive();

    /// Executed when the server receives a timeout from a client.
    /// @param[in] session - the session to start.
    void start_session(std::unique_ptr<client_session> session);

    /// Executed after response is sent to the client.
    void end_session(client_session* session);

private:
    asio::io_context& _io;
    asio::ip::udp::socket _socket;

    /// The set of active client_waiter objects.
    // std::unordered_set<std::shared_ptr<timed_session>> _sessions;
    std::unordered_map<client_session*, std::unique_ptr<client_session>> _sessions;
};

}// namespace nxudp

#endif //NX_UDP_SERVER_H
