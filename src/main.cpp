#include <iostream>
#include <asio.hpp>


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
    if(argc == 1)
    {
        help();
        return 0;
    }

    std::cout << "argc: " << argc << std::endl;
    std::cout << "argv: " << argv[0] << std::endl;
    return 0;
}