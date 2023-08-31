// This file uses Boost libraries for file path

#include <thread>
#include <boost/filesystem.hpp>
#include "argparse/argparse.hpp"
#include "ScoringEngine/ScoringEngine.h"
#include "ScoringEngine/EngineTypes.h"

int main(int argc, char* argv[])
{
    // init parser
    argparse::ArgumentParser parser("ScoringEngine", "0.1.0");

    parser.add_argument("answer_file")
        .help("path to answer file");

    parser.add_argument("submission_file")
        .help("path to submission file");

    parser.add_argument("-t", "--timeout")
        .help("timeout in seconds")
        .default_value(1.0f)
        .scan<'g', float>();

    parser.add_argument("-n", "--number")
        .help("number of iterations")
        .default_value(5)
        .scan<'i', int>();

    try
    {
        parser.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    SystemArgs sys_args{};

    sys_args.answer_file = boost::filesystem::canonical(parser.get<std::string>("answer_file")).string();
    sys_args.submission_file = boost::filesystem::canonical(parser.get<std::string>("submission_file")).string();
    sys_args.timeout = parser.get<float>("timeout");
    sys_args.number_of_iterations = parser.get<int>("number");


    // 결과를 저장하는 변수
    ScoringResult score_result{};


    // thread setting
    std::thread threads[sys_args.number_of_iterations];

    for (int i = 0; i < sys_args.number_of_iterations; i++)
    {
        threads[i] = std::thread([sys_args, &score_result]() -> void
        {
            ScoringEngine engine(sys_args, &score_result);
            engine.run();
        });
    }

    for (int i = 0; i < sys_args.number_of_iterations; i++)
    {
        threads[i].join();
    }


    // TODO: 출력결과 수정하기
    // print result
    std::cout << (
        score_result.accepted == sys_args.number_of_iterations ?
            ColorString::color_string("Accepted", ColorString::GREEN) :
            ColorString::color_string("Wrong Answer", ColorString::RED)
    ) << "\n";
    std::cout << "Accepted: " << score_result.accepted << ", "
              << "Wrong Answer: " << score_result.wrong_answer << ", "
              << "Time Out: " << score_result.timeout << std::endl;

    return 0;
}
