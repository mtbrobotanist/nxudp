//+
// Created by n8 on 3/16/18.
//

#ifndef NX_UDP_SERVER_H
#define NX_UDP_SERVER_H

#include <unordered_set>
#include <asio.hpp>
#include "int_buffer.h"
#include "export.h"

namespace nxudp
{

class client_waiter;


class NXUDP_API server
{
    typedef int_buffer receive_buffer;
    const std::string _RESPONSE = "DONE";

    typedef asio::ip::udp::endpoint endpoint;

public:
    /// The Constructor for the server object.
    /// @param[in] io - the asio::io_service object required to run the server's socket.
	server(asio::io_service& io);

	virtual ~server();

	/// Called by the given waiter to when it's timeout has completed.
    /// @param [in] waiter - the waiter that whose timer has expired.
    void wait_completed(const std::shared_ptr<client_waiter>& waiter);

private:

    /// The callback used by _socket.async_send_to().
    /// Erases the given client_waiter after a response has been sent to it's endpoint.
    /// @param [in] waiter - the waiter to remove from the server's list of active waiters.
    /// @param [in] message - the message to send over the socket to the waiter's endpoint.
    /// @param [in] error - provided by asio, An error code if one occured during the send operation.
    /// @param [in] bytes_transferred - provided by asio, the number of bytes sent out on the socket.
    void async_send_callback(const std::shared_ptr<client_waiter> waiter,
                             const std::string &message,
                             const asio::error_code &error,
                             std::size_t bytes_transferred);

    /// A helper function to parse the given buffer to a integer timeout value.
    /// @param[in] buffer - the buffer whose contents to convert to an integer.
    /// @param[in] size_t bytes_transferred, the number of bytes transferred transferred into @param buffer.
    /// @param[out] out_timeout - a reference to an integer that will contain the timeout value.
    /// @returns bool - the result of the conversion. true, if successful, false otherwise
    bool parse_timeout(receive_buffer &buffer, size_t bytes_transferred, int &out_timeout);

    /// The function that calls async_receive_from on the socket, specifying async_receive_callback() as the future function.
    void start_receive();

    /// the callback funciton,called by asio, on completion of the socket's async_receive_from function.
    void async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred);

    /// adds a client waiter from the maps
    void add_waiter(const std::shared_ptr<client_waiter>& waiter);

    /// removes a client_waiter from the map
    void remove_waiter(const std::shared_ptr<client_waiter>& waiter);

private:
    asio::io_service& _io;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _client_endpoint;
    int_buffer _receive_buffer;

    /// The set of active client_waiter objects.
    std::unordered_set<std::shared_ptr<client_waiter>> _waiters;

    std::mutex _waiters_mutex;

    std::mutex _socket_mutex;
};

}// namespace nxudp

#endif //NX_UDP_SERVER_H
