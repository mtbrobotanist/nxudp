#include <iostream>
#include <asio.hpp>
#include "program_options.h"
#include "client.h"
#include "server.h"

void helpExit()
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

void validateHost(const std::string& host)
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
            helpExit();
        }

    }
}

void getHostAndPort(const std::string& host_port, std::string& host, std::string& port)
{
    int colon = host_port.find(":");
    if(colon == 0 || colon == host_port.npos)
        helpExit();

    host = host_port.substr(0, colon);
    port = host_port.substr(colon + 1);
}

void serverMode()
{
    asio::io_service io;
    nxudp::server server(io);
    io.run();
}

void clientMode(const std::string& host_port, const std::string& msec)
{
    std::string host;
    std::string port;
    getHostAndPort(host_port, host, port);

    validateHost(host);

    asio::io_service io;
    nxudp::client client(io, host, port, msec);
    io.run();

    std::cout << "Exiting..." << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2 && argc != 5)
    {
        std::cout << argc << std::endl;
        helpExit();
    }

    nxudp::program_options options(argc, argv);

    std::string host_port;
    std::string msec;

    if(options.tryGetCmdOption("-c", host_port)
       && options.tryGetCmdOption("-n", msec))
    {
        clientMode(host_port, msec);
    }
    else if(options.cmdOptionExists("-s"))
    {
        serverMode();
    }
    else
    {
        helpExit();
    }

    return 0;
}