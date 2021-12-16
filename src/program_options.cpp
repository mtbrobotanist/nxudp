#include "program_options.h"

namespace nxudp
{

program_options::program_options(int argc, char **argv)
{
    if(argc > 1)
    {
        _tokens.reserve(argc - 1);

        for(int i = 1; i < argc; ++i)
            _tokens.push_back(std::string(argv[i]));
    }
}

bool program_options::get_value(const std::string &flag, std::string *out_value) const
{
    auto itr = std::find(_tokens.begin(), _tokens.end(), flag);
    if(itr == _tokens.end() || ++itr == _tokens.end())
        return false;

    if(out_value)
        *out_value = *itr;

    return true;
}

bool program_options::flag_exists(const std::string& flag) const
{
    return std::find(_tokens.begin(), _tokens.end(), flag) != _tokens.end();
}

bool program_options::empty() const
{
    return _tokens.empty();
}

std::size_t program_options::size() const
{
    return _tokens.size();
}

void program_options::add_validation(const std::string& flag, const std::regex& regex)
{
    _options.insert(std::pair<std::string, std::regex>(flag, regex));
}

bool program_options::validate(const std::string& flag, const std::string& value) const
{
    const auto& flag_regex = _options.find(flag);
    if(flag_regex == _options.end()) // no such option specified
        return false;

    return std::regex_match(value, flag_regex->second);
}

bool program_options::validate(const std::string& flag, std::string* out_value) const
{
    std::string value;
    if(!get_value(flag, &value))
        return false;

    if(!validate(flag, value))
        return false;

    if(out_value)
        *out_value = std::move(value);

    return true;
}

} // nxudp
