//
// Created by n8 on 3/16/18.
//

#include "server.h"
#include "print_stream.h"
#include "timed_session.h"


namespace nxudp
{

using asio::ip::udp;

server::server(asio::io_service& io) :
    network_object(io),
    _io(io)
{
    unsigned short port = _socket.local_endpoint().port();
    print_stream() << "Listening port " << port << "\n";

    start_receive();
}

server::~server()
{ }

void server::start_receive()
{
    auto func = std::bind(&server::async_receive_callback, this,
                          std::placeholders::_1,
                          std::placeholders::_2);

    // this sets the value of _client_endpoint when the before invoking the callback function
    _socket.async_receive_from(asio::buffer(_timeout_buffer), client_endpoint(), func);
}

void server::async_receive_callback(const asio::error_code &error, std::size_t bytes_transferred)
{
    int timeout;
    if (error || !parse_timeout(_timeout_buffer, bytes_transferred, timeout))
    {
        print_stream(std::cerr) << "The server received an invalid timeout from client: "<< client_endpoint()
                                << ". Ignoring...\n";
    }
    else
    {
        timeout_received(timeout, client_endpoint());
    }

    // we want to continue listening for other clients even if there was an error
    start_receive();
}

void server::async_send_callback(const std::shared_ptr<timed_session>& session,
                                 const std::string &message,
                                 const asio::error_code &error,
                                 std::size_t /*bytes_transferred*/)
{
    if(error)
    {
        print_stream(std::cerr) << "Error sending: " << error << "\n";
    }

    response_sent(session, message);
}

bool server::parse_timeout(int_buffer &buffer, size_t bytes_transferred, int &out_timeout)
{
    if(bytes_transferred < sizeof(int))
    {
        out_timeout = std::numeric_limits<int>::min();
        return false;
    }

    out_timeout = nxudp::utils::read_buffer(buffer);
    return true;
}

void server::end_session(const std::shared_ptr<timed_session>& session)
{
    auto func = std::bind(&server::async_send_callback, this,
                            session,
                            _RESPONSE,
                            std::placeholders::_1,
                            std::placeholders::_2);

    _socket.async_send_to(asio::buffer(_RESPONSE), session->client_endpoint(), func);
}

void server::add_session(const std::shared_ptr<timed_session>& session)
{
    _sessions.insert(session);
}

void server::remove_session(const std::shared_ptr<timed_session>& session)
{
    _sessions.erase(session);
}

asio::ip::udp::endpoint& server::client_endpoint()
{
    return _remote_endpoint;
}

void server::timeout_received(int timeout, const asio::ip::udp::endpoint& client_endpoint)
{
    std::shared_ptr<timed_session> session = std::make_shared<timed_session>(*this, _io, session_data(client_endpoint, timeout));

    add_session(session);

    session->start();

    print_stream() << "Received request from " << client_endpoint << " with value \"" << timeout << "\"" << std::endl;
}

void server::response_sent(const std::shared_ptr<timed_session>& session, const std::string& message)
{
    remove_session(session);

    print_stream() << "Sent response \"" << message << "\" to "<< session->client_endpoint() << std::endl;
}

}// namespace nxudp