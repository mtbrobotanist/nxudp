//
// Created by n8 on 3/15/18.
//

#ifndef NX_UDP_PROGRAMOPTIONS_H
#define NX_UDP_PROGRAMOPTIONS_H

#include <algorithm>
#include <vector>

namespace nxudp
{
    class ProgramOptions
    {
    public:
        ProgramOptions(int &argc, char **argv)
        {
            for (int i = 1; i < argc; ++i)
                _tokens.push_back(std::string(argv[i]));
        }

        const std::string getCmdOption(const std::string &option) const
        {
            std::vector<std::string>::const_iterator itr;
            itr = std::find(_tokens.begin(), _tokens.end(), option);

            if (itr != _tokens.end() && ++itr != _tokens.end())
                return *itr;

            return std::string();
        }

        bool cmdOptionExists(const std::string &option) const
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
