#pragma once
#include <string>

struct SystemArgs
{
    std::string answer_file;
    std::string submission_file;
    float timeout;
    int number_of_iterations;
};

struct ScoringResult
{
    ScoringResult() : accepted(0), wrong_answer(0), timeout(0){};

    int accepted;
    int wrong_answer;
    int timeout;
};

namespace SupportFileExtention
{
    /// @brief 확장자에 맞춰 명령어를 작성
    /// @param ext 파일 확장자
    /// @param cmd 실행할 명령어
    void insert_prefix_cmd(const std::string& ext, std::string& cmd)
    {
        if (ext == ".exe")
        {
            return;
        }
        else if (ext == ".py")
        {
            cmd = "python.exe \"" + cmd + "\"";
        }
    }
};

namespace ColorString
{
    enum EColor
    {
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        END
    };

    static const char* colors[] = {
        "\033[31m",
        "\033[32m",
        "\033[33m",
        "\033[34m",
        "\033[35m",
        "\033[36m",
        "\033[37m",
        "\033[0m"
    };


    /// @brief 문자열에 색을 적용함
    /// @param str 적용할 문자열
    /// @param color 적용할 색깔
    /// @return 색깔이 적용된 문자열
    std::string color_string(const std::string& str, EColor color)
    {
        return colors[color] + str + colors[END];
    }
} // namespace ColorString