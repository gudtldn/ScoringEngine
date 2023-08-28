import subprocess
from os import path
from argparse import ArgumentParser
from threading import Thread
from typing import TypedDict

from tools.color_text import *


class SystemArgs:
    submission_file: str
    answer_file: str
    use_submission_input: bool
    number: int
    timeout: float


class ScoreResult(TypedDict):
    accepted: int
    wrong: int
    timeout: int


def main(answer_file: str, submission_file: str, score_result: ScoreResult, use_submission_input: bool = False, timeout: float = 1.0):
    try:
        if use_submission_input:
            with subprocess.Popen(["python", answer_file], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as answer_process:
                answer_stdout, _ = answer_process.communicate()
                answer_splited = answer_stdout.split("|")
                input_value = answer_splited[:-1]
                answer_stdout = answer_splited[-1]

            with subprocess.Popen(["python", submission_file], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as submission_process:
                submission_stdout, _ = submission_process.communicate(input="\n".join(input_value), timeout=timeout)

        else:
            with subprocess.Popen(["python", answer_file], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as answer_process:
                answer_stdout, _ = answer_process.communicate()

            with subprocess.Popen(["python", submission_file], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as submission_process:
                submission_stdout, _ = submission_process.communicate(timeout=timeout)

    except subprocess.TimeoutExpired:
        submission_process.kill()
        score_result['timeout'] += 1

    except Exception as e:
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
    parser.add_argument("-i", "--input", dest="use_submission_input", help="use submission input", action="store_true")
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
                args.use_submission_input,
                args.timeout
            )
        ) for _ in range(args.number if args.use_submission_input else 1)
    ]

    for t in thread:
        t.start()

    for t in thread:
        t.join()

    print(
        color_text("accepted", ETextColor.GREEN) if score_result['accepted'] == sum(score_result.values()) else color_text("wrong answer", ETextColor.RED),
        f"\n{score_result}"
    )
