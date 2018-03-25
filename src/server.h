//+
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_SERVER_H
#define NX_UDP_SERVER_H

#include <unordered_set>
#include <asio.hpp>
#include "int_buffer.h"
#include "export.h"
#include "network_object.h"

namespace nxudp
{

    class timed_session;

    /// This class is responsible for waiting for input on a socket, parsing the timeout, and constructing timed sessions.
    /// when a timed session expires, it sends a reponse back to the client that initiated the timed session.
class NXUDP_API server : network_object
{
    const std::string _RESPONSE = "DONE";

public:
    /// The Constructor for the server object.
    /// @param[in] io - the asio::io_service object required to run the server's socket.
	server(asio::io_service& io);

	virtual ~server();

	/// Called by the given waiter to when it's timeout has completed.
    /// @param [in] session - the waiter that whose timer has expired.
    void end_session(const std::shared_ptr<timed_session>& session);
private:

    /// The callback used by _socket.async_send_to().
    /// Erases the given client_waiter after a response has been sent to it's endpoint.
    /// @param [in] session - the waiter to remove from the server's list of active waiters.
    /// @param [in] message - the message to send over the socket to the waiter's endpoint.
    /// @param [in] error - provided by asio, An error code if one occured during the send operation.
    /// @param [in] bytes_transferred - provided by asio, the number of bytes sent out on the socket.
    void async_send_callback(const std::shared_ptr<timed_session>& session,
                             const std::string &message,
                             const asio::error_code &error,
                             std::size_t bytes_transferred);

    /// A helper function to parse the given buffer to a integer timeout value.
    /// @param[in] buffer - the buffer whose contents to convert to an integer.
    /// @param[in] size_t bytes_transferred, the number of bytes transferred transferred into @param buffer.
    /// @param[out] out_timeout - a reference to an integer that will contain the timeout value.
    /// @returns bool - the result of the conversion. true, if successful, false otherwise
    bool parse_timeout(int_buffer &buffer, size_t bytes_transferred, int &out_timeout);

    /// The function that calls async_receive_from() on the socket, specifying async_receive_callback() as the future function.
    void start_receive();

    /// The callback funciton,called by asio, on completion of the socket's async_receive_from() function.
    /// @param[in] error - the asio error code, provided by asio.
    /// @param[in] bytes_transferred - the number of bytes written to the socket, provided by asio.
    void async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred);

    /// Adds a timed_session to the sever, which will eventually send a response back to the client on the session's behalf
    /// @param[in] session - the session to add.
    void add_session(const std::shared_ptr<timed_session>& session);

    /// Removes a client_waiter from the map
    /// @param[in] session - the session to remove.
    void remove_session(const std::shared_ptr<timed_session>& session);

    /// Get the current client endpoint as determined by the most recent client packet received
    asio::ip::udp::endpoint& client_endpoint();

    /// Executed when the server receives a timeout from a client
    /// @param[in] timeout - the timeout received from a client
    /// @param[in] client_endpoint - the client_endpoint associated with the timeout
    void timeout_received(int timeout, const asio::ip::udp::endpoint& client_endpoint);


    /// Executed when the server has sent a response to the client represented by the given session
    /// @param[in] session - the timed_session representing the client
    void response_sent(const std::shared_ptr<timed_session>& session, const std::string& message);

private:
    asio::io_service& _io;

    /// The set of active client_waiter objects.
    std::unordered_set<std::shared_ptr<timed_session>> _sessions;
};

}// namespace nxudp

#endif //NX_UDP_SERVER_H
