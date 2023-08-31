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

    std::string color_string(const std::string& str, EColor color)
    {
        return colors[color] + str + colors[END];
    }
} // namespace ColorString