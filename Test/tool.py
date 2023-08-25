import subprocess

with subprocess.Popen(["python", "./Test/answer.py"], stdout=subprocess.PIPE, text=True) as proc:
    stdout, _ = proc.communicate()
    print(repr(stdout))