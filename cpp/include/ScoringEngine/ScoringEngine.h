#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "ScoringEngine/EngineTypes.h"
#include "ScoringEngine/StringHelper.h"
#include "ScoringEngine/Process.h"

namespace bp = boost::process;

class ScoringEngine
{
private:
    SystemArgs sys_args;
    ScoringResult& result;
    std::mutex& result_mutex;

public:
    ScoringEngine(const SystemArgs& in_args, ScoringResult& in_result, std::mutex& in_mutex) : sys_args(in_args), result(in_result), result_mutex(in_mutex){};

    void run();
};

void ScoringEngine::run()
{
    // answer_process
    std::string answer_stdout;
    SupportFileExtention::insert_prefix_cmd(
        StringHelper::get_file_ext(sys_args.answer_file),
        sys_args.answer_file
    );

    Process answer_process(sys_args.answer_file, 10.0f);
    answer_process.run(answer_stdout);

    std::vector<std::string> answer_stdout_splitted = StringHelper::split(answer_stdout, '|');
    std::string answer = answer_stdout_splitted[answer_stdout_splitted.size()-1];
    std::string submission_stdin;

    for (size_t i = 0; i < answer_stdout_splitted.size()-1; i++)
    {
        submission_stdin += StringHelper::ltrim(answer_stdout_splitted[i]) + "\n";
    }


    // submission_process
    std::string submission_stdout;
    SupportFileExtention::insert_prefix_cmd(
        StringHelper::get_file_ext(sys_args.submission_file),
        sys_args.submission_file
    );

    Process submission_process(sys_args.submission_file, sys_args.timeout);
    submission_process.run(submission_stdout, submission_stdin);


    // update result
    result_mutex.lock();
    if (submission_process.is_timeout())
    {
        result.timeout++;
    }
    else if (answer == submission_stdout)
    {
        result.accepted++;
    }
    else
    {
        result.wrong_answer++;
    }
    result_mutex.unlock();
}
