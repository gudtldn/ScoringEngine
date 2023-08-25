import subprocess
from os import path
from argparse import ArgumentParser
from threading import Thread
from typing import TypedDict


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
    if use_submission_input:
        # TODO: 입력값 만들기
        ...

    else:
        with subprocess.Popen(["python", answer_file], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as answer_process:
            answer_stdout, _ = answer_process.communicate()

        with subprocess.Popen(["python", submission_file], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as submission_process:
            try:
                submission_stdout, _ = submission_process.communicate(timeout=timeout)

            except subprocess.TimeoutExpired:
                submission_process.kill()
                score_result['timeout'] += 1
                submission_stdout = None

            except:
                score_result['wrong'] += 1

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
    parser.add_argument("-t", "--timeout", dest="timeout", help="timeout second", default=1, type=float)
    parser.add_argument("-n", "--number", dest="number", help="number of test cases", default=10, type=int)
    args: SystemArgs = parser.parse_args()

    score_result: ScoreResult = {
        "accepted": 0,
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

    print("accepted" if score_result['accepted'] == sum(score_result.values()) else "wrong answer", f"({score_result['accepted']}/{sum(score_result.values())})")
