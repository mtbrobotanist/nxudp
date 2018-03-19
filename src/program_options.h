//
// Created by n8 on 3/15/18.
//

#ifndef NX_UDP_PROGRAMOPTIONS_H
#define NX_UDP_PROGRAMOPTIONS_H

#include <algorithm>
#include <vector>
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
    program_options(int &argc, char **argv)
    {
        if(argc > 1)
        {
            _tokens.reserve(argc - 1);

            for (int i = 1; i < argc; ++i)
                _tokens.push_back(std::string(argv[i]));
        }
    }

	virtual ~program_options(){}

    /// Try to get the value of a command option flag.
    /// @param[in] option - the command option whose value to get, for example, "-n"
    /// @param[in] out_value - the value associated with the command option flag.
    /// @returns bool - true if there was a command value following the option in the argumensts, false otherwise.
    bool try_get_cmd_option(const std::string &option, std::string &out_value) const
    {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(_tokens.begin(), _tokens.end(), option);

        if (itr != _tokens.end())
        {
            out_value = ++itr != _tokens.end() ? *itr : std::string();
            return true;
        }

        out_value = std::string();
        return false;
    }

    /// Find out if a particular command option exists.
    /// @param[in] option - the option that may or may not exist.
    /// @returns bool - true if the option exists, false otherwise.
    bool cmd_option_exists(const std::string &option)
    {
        return std::find(_tokens.begin(), _tokens.end(), option) != _tokens.end();
    }

    bool empty()
    {
        return _tokens.size() == 0;
    }

private:

    /// The list of raw arguments.
    std::vector<std::string> _tokens;
};

}// namespace nxudp

#endif //NX_UDP_PROGRAMOPTIONS_H
