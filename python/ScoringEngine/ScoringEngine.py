import subprocess
from os import path
from argparse import ArgumentParser
from threading import Thread
from typing import TypedDict

from tools.color_text import *
from tools.support_file_extention import *


class SystemArgs:
    submission_file: str
    answer_file: str
    number: int
    timeout: float


class ScoreResult(TypedDict):
    accepted: int
    wrong: int
    timeout: int


def main(answer_file: str, submission_file: str, score_result: ScoreResult, timeout: float = 1.0):
    try:
        answer_cmd = [answer_file]
        insert_prefix(path.splitext(answer_file)[1], answer_cmd)

        with subprocess.Popen(answer_cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as answer_process:
            answer_stdout, _ = answer_process.communicate()
            answer_splited = answer_stdout.split("|")
            input_value = map(lambda x: x.lstrip(), answer_splited[:-1])
            answer_stdout = answer_splited[-1]

        submission_cmd = [submission_file]
        insert_prefix(path.splitext(submission_file)[1], submission_cmd)

        submission_process = subprocess.Popen(submission_cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
        submission_stdout, _ = submission_process.communicate(input="\n".join(input_value), timeout=timeout)

    except subprocess.TimeoutExpired:
        submission_process.kill()
        score_result['timeout'] += 1

    except Exception as e:
        print(e)
        submission_process.kill()
        raise e

    else:
        if answer_stdout == submission_stdout:
            score_result['accepted'] += 1
        else:
            score_result['wrong'] += 1


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("answer_file", help="path to answer file", type=str)
    parser.add_argument("submission_file", help="path to submission file", type=str)
    parser.add_argument("-t", "--timeout", dest="timeout", help="timeout second", default=1.0, type=float)
    parser.add_argument("-n", "--number", dest="number", help="number of iterations", default=5, type=int)
    args: SystemArgs = parser.parse_args()

    score_result: ScoreResult = {
        "accepted": 0,
        "wrong": 0,
        "timeout": 0
    }

    thread = [
        Thread(
            target=main,
            args=(
                path.abspath(args.answer_file),
                path.abspath(args.submission_file),
                score_result,
                args.timeout
            )
        ) for _ in range(args.number)
    ]

    for t in thread:
        t.start()

    for t in thread:
        t.join()


    result_text = ""
    if args.number != 0 and sum(score_result.values()) == 0:
        result_text = color_text("EngineError", ETextColor.RED)

    elif score_result['accepted'] == args.number:
        result_text = f"{color_text('accepted', ETextColor.GREEN)}\n{score_result}"

    elif score_result['timeout'] > 0:
        result_text = f"{color_text('timeout', ETextColor.RED)}\n{score_result}"

    else:
        result_text = f"{color_text('wrong answer', ETextColor.RED)}\n{score_result}"

    print(result_text)
