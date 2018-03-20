//
// Created by n8 on 3/15/18.
//

#ifndef NX_UDP_PROGRAMOPTIONS_H
#define NX_UDP_PROGRAMOPTIONS_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <regex>
#include "export.h"

namespace nxudp
{

/// A helper class that makes it easier to parse command line arguments.
/// This is a very simple implementation that does not validate the value associated with a given command option.
/// For example, for the following program invocation:
///
///
///     ./app -c -n 1500
///
/// if you were to call options.try_get_cmd_option("-c"), you would receive "-n" as the value, which is probably not what you want.
class NXUDP_API program_options
{
public:
    /// Constructs a program_options object
    /// @param[in] argc - the argument count.
    /// @param [in] argv - the array of string args.
    program_options(int &argc, char **argv);

    virtual ~program_options();

    /// Try to get the value of a command option flag. "Value" in this case is simply the argumeng following the flag
    /// @param[in] option - the command option whose value to get, for example, "-n"
    /// @param[in] out_value - the value associated with the command option flag.
    /// @returns bool - true if there was a command value following the option in the argumensts, false otherwise.
    bool get_value(const std::string &option, std::string &out_value) const;

    /// Find out if a particular command option exists.
    /// @param[in] option - the option that may or may not exist.
    /// @returns bool - true if the option exists, false otherwise.
    bool flag_exists(const std::string &option) const;

    /// Determine if the command line args is empty (disregarding the default first parameter)
    bool empty() const;

    /// Add a regular expression match for the given flag
    /// @param[in] flag - the flag to add validation for
    /// @param[in] regex - the regex that performs the validation
    void add_validation(const std::string& flag, const std::regex& regex);
    
    /// validates the next argument in the argument list against the stored regular expression   
    /// @param[in] flag - the flag whose regex to validate against
    /// @param[in] value - the value to validate
    bool validate(const std::string& flag, const std::string& value) const;
    
private:

    /// The list of raw arguments.
    std::vector<std::string> _tokens;
    
    /// The set of flags and the regular expressions for their associated values
    std::unordered_map<std::string, std::regex> _options;
};

}// namespace nxudp

#endif //NX_UDP_PROGRAMOPTIONS_H
