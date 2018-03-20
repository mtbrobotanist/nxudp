
/* 
 * File:   endpoint_utils.cpp
 * Author: n8
 * 
 * Created on March 19, 2018, 6:30 PM
 */

#include "udp_endpoint_utils.h"

namespace nxudp
{
namespace utils 
{    
namespace udp
{
    
using asio::ip::udp;    

bool resolve_endpoint(asio::io_service& io, const std::string& host, const std::string& port, udp::endpoint& out_endpoint, std::string* out_error)
{
    udp::resolver resolver(io);
    udp::resolver::query query(udp::v4(), host, port);

    asio::error_code error;
    udp::resolver::iterator it = resolver.resolve(query, error);
    udp::resolver::iterator end;

    if(error)
    {
        if(out_error)
        {
            *out_error= "Invalid host:port combination provided." + host + ":" + port;
        }
        
        return false;
    }

    if(it == end) // invalid host and port provided.
    {
        if(out_error)
        {
            *out_error = "No matching host:port combination found.";
        }
        
        return  false;
    }

    out_endpoint = *it;
    return true;
}

}
} // namespace utils
} // namespace nxudp