#include <memory>
#include <regex>
#include <asio.hpp>
#include <csignal>
#include "asio/io_service.hpp"
#include "program_options.h"
#include "endpoint_utils.h"
#include "client.h"
#include "server.h"
#include "print_stream.h"


using nxudp::stdcout;
using nxudp::stdcerr;

std::shared_ptr<asio::io_service> io;
bool running_server = false;


void signal_handler(int signal)
{
    io->stop();

    const char* stop_message = running_server ? "Stopping server." : "Stopping client.";

    stdcout() << stop_message << std::endl;
}

void help()
{
    stdcout() << "Usage:\n"
        << "\n"
        << "   Client Mode\n"
        << "       app -c <ip-address>:<port> -n <milliseconds>\n"
        << "\n"
        << "   Server Mode\n"
        << "       app -s\n"
        << std::endl;
}

void server_mode()
{
    running_server = true;

    nxudp::server server(*io);

    io->run();
}

void client_mode(const std::string& host_port, const std::string& milliseconds)
{
    std::size_t colon = host_port.find(":"); //< guaranteed by regex validation.

    std::string host = host_port.substr(0, colon);
    std::string port = host_port.substr(colon + 1);

    std::string error;
    asio::ip::udp::endpoint server_endpoint;
    if(nxudp::utils::resolve_endpoint(*io, host, port, server_endpoint, &error))
    {
        nxudp::client client(*io, server_endpoint, std::stoi(milliseconds));
        io->run();
    }
    else
    {
        stdcerr() << error << std::endl;
    }
}

void add_command_line_validation(nxudp::program_options& options)
{
    options.add_validation("-s", std::regex()); //< standalone server flag
    options.add_validation("-c", std::regex("([^,]*):\\d+")); //< [host-name | ip-address]:port combination
    options.add_validation("-n", std::regex("\\d+")); //< millisecond count
}

int main(int argc, char* argv[])
{
    io = std::make_shared<asio::io_service>();
    std::signal(SIGINT, signal_handler);

    nxudp::program_options options(argc, argv);
    add_command_line_validation(options);

    if(options.flag_exists("-s"))
    {
        if(options.size() == 1)
            server_mode();
        else //< server mode must have no other flags or values.
            help();
    }
    else
    {
        std::string host_port;
        std::string msec;
        if(options.validate("-c", &host_port) && options.validate("-n", &msec))
            client_mode(host_port, msec);
        else
            help();
    }

    return 0;
}