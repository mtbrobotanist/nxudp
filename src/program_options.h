//
// Created by n8 on 3/15/18.
//

#ifndef NX_UDP_PROGRAMOPTIONS_H
#define NX_UDP_PROGRAMOPTIONS_H

#include <algorithm>
#include <vector>

namespace nxudp
{
    class program_options
    {
    public:
        program_options(int &argc, char **argv)
        {
            for (int i = 1; i < argc; ++i)
                _tokens.push_back(std::string(argv[i]));
        }

        bool try_get_cmd_option(const std::string &option, std::string &arg_out) const
        {
            std::vector<std::string>::const_iterator itr;
            itr = std::find(_tokens.begin(), _tokens.end(), option);

            if (itr != _tokens.end())
            {
                arg_out = ++itr != _tokens.end() ? *itr : std::string();
                return true;
            }

            arg_out = std::string();
            return false;
        }

        bool cmd_option_exists(const std::string &option)
        {
            return std::find(_tokens.begin(), _tokens.end(), option) != _tokens.end();
        }

        bool empty()
        {
            return _tokens.size() == 0;
        }

    private:
        std::vector<std::string> _tokens;
    };
}

#endif //NX_UDP_PROGRAMOPTIONS_H
