// This file uses Boost libraries for string manipulation.

#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/trim.hpp>

namespace StringHelper
{
    /// @brief 문자열을 delimiter를 기준으로 분리해 vector로 분리
    /// @param str 분리할 문자열
    /// @param delimiter 분리할 문자
    /// @return delimiter를 기준으로 분리된 vector<string>
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


    /// @brief 문자열 처음과 끝에 있는 공백 및 '\\n', '\\r'을 제거
    /// @param str trim할 문자열
    /// @return trim된 문자열
    std::string trim(const std::string& str)
    {
        std::string result = str;
        boost::algorithm::trim(result);
        return result;
    }


    /// @brief 문자열 처음에 있는 공백 및 '\\n', '\\r'을 제거
    /// @param str trim할 문자열
    /// @return trim된 문자열
    std::string ltrim(const std::string& str)
    {
        std::string result = str;
        boost::algorithm::trim_left(result);
        return result;
    }


    /// @brief 문자열 끝에 있는 공백 및 '\\n', '\\r'을 제거
    /// @param str trim할 문자열
    /// @return trim된 문자열
    std::string rtrim(const std::string& str)
    {
        std::string result = str;
        boost::algorithm::trim_right(result);
        return result;
    }


    /// @brief 문자열에서 '\\r'을 제거
    /// @param str '\\r'을 제거할 문자열
    /// @return '\\r'이 제거된 문자열
    std::string remove_carriage_return(const std::string& str)
    {
        std::string result = str;
        result.erase(
            std::remove(result.begin(), result.end(), '\r'),
            result.end()
        );
        return result;
    }

    // file path 관련

    /// @brief 파일을 파일명과 확장자로 분리
    /// @param path 파일 경로
    /// @return 파일명, 확장자
    std::pair<std::string, std::string> splitext(const std::string &path)
    {
        size_t lastDot = path.find_last_of('.');
        if (lastDot == std::string::npos)
        {
            return std::make_pair(path, "");
        }
        return std::make_pair(path.substr(0, lastDot), path.substr(lastDot));
    }


    /// @brief 파일의 이름을 반환
    /// @param path 파일 경로
    /// @return 파일 이름
    std::string get_file_name(const std::string &path)
    {
        return splitext(path).first;
    }


    /// @brief 파일의 확장자를 반환
    /// @param path 파일 경로
    /// @return 파일 확장자
    std::string get_file_ext(const std::string &path)
    {
        return splitext(path).second;
    }
} // namespace String
