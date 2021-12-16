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


std::shared_ptr<asio::io_service> io;
bool running_server = false;


void signal_handler(int signal)
{
    io->stop();

    std::string stop_message = running_server ? "Stopping server." : "Stopping client.";

    nxudp::print_stream() << stop_message << std::endl;
}

void help()
{
    std::string help = "Usage:\n";
    help += "\n";
    help += "   Client Mode\n";
    help += "       app -c <ip-address>:<port> -n <milliseconds>\n";
    help += "\n";
    help += "   Server Mode\n";
    help += "       app -s\n";

    nxudp::print_stream() << help << "\n";
}

void server_mode()
{
    running_server = true;

    nxudp::server server(*io);

    io->run();
}

void get_host_and_port(const std::string& host_port, std::string& host, std::string& port)
{
    std::size_t colon = host_port.find(":");

    host = host_port.substr(0, colon);
    port = host_port.substr(colon + 1);
}

void client_mode(const std::string& host_port, const std::string& milliseconds)
{
    running_server = false;

    std::string host;
    std::string port;
    get_host_and_port(host_port, host, port);

    asio::ip::udp::endpoint server_endpoint;

    std::string error;
    if(nxudp::utils::resolve_endpoint(*io, host, port, server_endpoint, &error))
    {
        nxudp::client client(*io, server_endpoint, std::stoi(milliseconds));
        io->run();
    }
    else
    {
        nxudp::print_stream(std::cerr) << error << "\n";
    }
}

void add_command_line_validation(nxudp::program_options& options)
{
    options.add_validation("-s", std::regex()); // standalone server flag;

    options.add_validation("-c", std::regex("([^,]*):\\d+")); // [host-name | ip-address]:port combination

    options.add_validation("-n", std::regex("\\d+")); // millisecond count
}

int main(int argc, char* argv[])
{
    io = std::make_shared<asio::io_service>();
    std::signal(SIGINT, signal_handler);

    nxudp::program_options options(argc, argv);
    add_command_line_validation(options);

    // server mode must have no other flags or values.
    if (options.flag_exists("-s") && options.size() > 1)
    {
        help();
        return 0;
    }

    std::string host_port;
    std::string msec;
    if(options.validate("-c", &host_port) && options.validate("-n", &msec))
        client_mode(host_port, msec);
    else if(options.flag_exists("-s") && !options.get_value("-s")) //< -s should not have value, it may be a mistake e.g. user entered -s localhost:12345.
        server_mode();
    else
     help();

    return 0;
}