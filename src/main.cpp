#include <iostream>
#include <regex>
#include <asio.hpp>
#include "program_options.h"
#include "endpoint_utils.h"
#include "client.h"
#include "server.h"
#include "session_data.h"
#include "print_stream.h"

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


/**
 * Note multi threading really isn't needed here.
 * The server runs fine with a single thread because it
 * uses async functions. I did this for fun.
 */
void run_server_mode_threads(asio::io_service& io)
{
    unsigned cpu_count =
            std::max<unsigned>(std::thread::hardware_concurrency(), 1);

    std::vector<std::thread> threads;
    threads.reserve(cpu_count);

    for(int i = 0; i < cpu_count; ++i)
    {
        threads.push_back(std::thread([&io](){ io.run(); }));
    }

    for(std::thread& t : threads)
    {
        t.join();
    }
}

void server_mode()
{
    asio::io_service io;
    nxudp::server server(io);

    run_server_mode_threads(io);
}


void get_host_and_port(const std::string& host_port, std::string& host, std::string& port)
{
    int colon = host_port.find(":");

    host = host_port.substr(0, colon);
    port = host_port.substr(colon + 1);
}

void client_mode(const std::string& host_port, const std::string& milliseconds)
{
    std::string host;
    std::string port;
    get_host_and_port(host_port, host, port);

    asio::io_service io;
    asio::ip::udp::endpoint server_endpoint;

    std::string error;
    if(nxudp::utils::resolve_endpoint(io, host, port, server_endpoint, &error))
    {
        nxudp::client client(io, nxudp::session_data(server_endpoint, std::stoi(milliseconds)));
        io.run();
    }
    else
    {
        nxudp::print_stream() << error << "\n";
    }
}

void add_command_line_validation(nxudp::program_options& options)
{
    options.add_validation("-s", std::regex()); // standalone server flag;

    options.add_validation("-c", std::regex("([^,]*):\\d+")); // [host-name | ip-address]:port combination
    
    options.add_validation("-n", std::regex("\\d+"));
}

int main(int argc, char* argv[])
{
    nxudp::program_options options(argc, argv);
    
    add_command_line_validation(options);
    
    std::string host_port;
    std::string msec;

    if(options.get_value("-c", host_port) && options.get_value("-n", msec)
       && options.validate("-c", host_port) && options.validate("-n", msec))
    {
        client_mode(host_port, msec);
    }
    else if(options.flag_exists("-s"))
    {
        server_mode();
    }
    else
    {
        help();
    }

    return 0;
}