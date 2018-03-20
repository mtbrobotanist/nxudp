#include <iostream>
#include <regex>
#include <asio.hpp>
#include "program_options.h"
#include "client.h"
#include "server.h"

void help_exit()
{
    std::string help = "Usage:\n";
    help += "\n";
    help += "   Client Mode\n";
    help += "       app -c <ip-address>:<port> -n <milliseconds>\n";
    help += "\n";
    help += "   Server Mode\n";
    help += "       app -s\n";

    std::cout << help << std::endl;

    exit(0);
}


void server_mode()
{
    asio::io_service io;
    nxudp::server server(io);
    io.run();
}


void get_host_and_port(const std::string &host_port, std::string &host, std::string &port)
{
    int colon = host_port.find(":");
    if(colon == 0 || colon == host_port.npos)
        help_exit();

    host = host_port.substr(0, colon);
    port = host_port.substr(colon + 1);
}

void client_mode(const std::string &host_port, const std::string &milliseconds)
{
    std::string host;
    std::string port;
    get_host_and_port(host_port, host, port);

    asio::io_service io;
    nxudp::client client(io, host, port, milliseconds);
    io.run();

    std::cout << "Exiting..." << std::endl;
    io.stop();
}


int main(int argc, char* argv[])
{
    if(argc != 2 && argc != 5)
    {
        help_exit();
    }

    nxudp::program_options options(argc, argv);

    std::string host_port;
    std::string msec;

    if(options.get_cmd_option("-c", host_port)
       && options.get_cmd_option("-n", msec))
    {
        client_mode(host_port, msec);
    }
    else if(options.cmd_option_exists("-s"))
    {
        server_mode();
    }
    else
    {
        help_exit();
    }

    return 0;
}