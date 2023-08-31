// This file uses Boost libraries for string manipulation.

#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/trim.hpp>

namespace StringHelper
{
    std::vector<std::string> split(const std::string& str, char delimiter)
    {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string item;

        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }

        return result;
    }
    
    std::string trim(const std::string& str)
    {
        std::string result = str;
        boost::algorithm::trim(result);
        return result;
    }

    std::string ltrim(const std::string& str)
    {
        std::string result = str;
        boost::algorithm::trim_left(result);
        return result;
    }

    std::string rtrim(const std::string& str)
    {
        std::string result = str;
        boost::algorithm::trim_right(result);
        return result;
    }

    // file path 관련
    std::pair<std::string, std::string> splitext(const std::string &path)
    {
        size_t lastDot = path.find_last_of('.');
        if (lastDot == std::string::npos)
        {
            return std::make_pair(path, "");
        }
        return std::make_pair(path.substr(0, lastDot), path.substr(lastDot));
    }

    std::string get_file_name(const std::string &path)
    {
        return splitext(path).first;
    }

    std::string get_file_ext(const std::string &path)
    {
        return splitext(path).second;
    }
} // namespace String
