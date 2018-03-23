
/* 
 * File:   endpoint_utils.h
 * Author: n8
 *
 * Created on March 19, 2018, 6:30 PM
 */

#ifndef ENDPOINT_UTILS_H
#define ENDPOINT_UTILS_H

#include <asio.hpp>
#include "export.h"

namespace nxudp
{
namespace utils 
{
 
/// A helper function that resolves the host and port information to a proper endpoint.
/// Returns true if successful. false otherwise.
/// @param[in] io - the io_service required to perform the endpoint query.
/// @param[in] host - the host to resolve.
/// @param[in] port - the port to resovle.
/// @param[out] out_endpoint - a reference to an endpoint that will be assigned the resolved endpoint if successful
/// @param[out] out_error - a readable string error, if there was one
bool NXUDP_API resolve_endpoint(asio::io_service& io, 
                                const std::string& host, 
                                const std::string& port,
                                asio::ip::udp::endpoint& out_endpoint, 
                                std::string* out_error = nullptr);
} // namespace utils
} // namespace nxudp

#endif /* ENDPOINT_UTILS_H */

