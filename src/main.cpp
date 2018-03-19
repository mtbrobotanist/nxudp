#include <iostream>
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

void validate_host(const std::string &host)
{
    if(host.find('.') != host.npos)
    {
        int occurrences = 0;
        std::string::size_type pos = 0;
        std::string target = ".";
        while ((pos = host.find(target, pos )) != std::string::npos) {
            ++ occurrences;
            pos += target.length();
        }

        if(occurrences != 3)
        {
            std::cout << "Invalid ip address: " << host << std::endl;
            help_exit();
        }

    }
}

int parse_milliseconds(std::string msec)
{
    try
    {
        return std::stoi(msec);
    }
    catch(std::exception& e)
    {
        std::cout << "Error: " << msec << "is not a valid value for milliseconds" << std::endl;
        help_exit();
    }

    return 0;
}

void client_mode(const std::string &host_port, const std::string &milliseconds)
{
    std::string host;
    std::string port;
    get_host_and_port(host_port, host, port);

    validate_host(host);

    int msec = parse_milliseconds(milliseconds);

    asio::io_service io;
    nxudp::client client(io, host, port, msec);
    io.run();

    std::cout << "Exiting..." << std::endl;
    io.stop();
}


int main(int argc, char* argv[])
{
    if(argc != 2 && argc != 5)
    {
        std::cout << argc << std::endl;
        help_exit();
    }

    nxudp::program_options options(argc, argv);

    std::string host_port;
    std::string msec;

    if(options.try_get_cmd_option("-c", host_port)
       && options.try_get_cmd_option("-n", msec))
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