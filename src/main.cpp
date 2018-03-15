#include <iostream>
#include <asio.hpp>
#include "ProgramOptions.h"


void help()
{
    std::string help = "Usage:\n";
    help += "\n";
    help += "   Client Mode\n";
    help += "       app -c <ip-address>:<port> -n <milliseconds>\n";
    help += "\n";
    help += "   ServerMode\n";
    help += "       app -s\n";

    std::cout << help << std::endl;
}

int main(int argc, char* argv[])
{
    nxudp::ProgramOptions options(argc, argv);

    if(options.empty())
    {
        help();
        return 0;
    }


    if(options.cmdOptionExists("-c")
       && options.cmdOptionExists("-n"))
    {
        //client mode
    }
    else if(options.cmdOptionExists("-s"))
    {
        //server mode
    }


    std::cout << "argc: " << argc << std::endl;
    std::cout << "argv: " << argv[0] << std::endl;
    return 0;
}