# ScoringEngine C++

## 사용된 라이브러리

- [p-ranav/argparse](https://github.com/p-ranav/argparse)
- [boost library 1.83.0](https://www.boost.org/)


## 현재 채점 가능한 언어

- Python
- .exe로 이루어진 파일


## 정답 파일 작성 방법

채점 파일에 입력할 값을 출력하고, 끝에 `|`를 붙인다.    
그리고 마지막에 정답을 입력한다.

> 예시
> ```py
> from random import randint
> 
> a = randint(-10, 10)
> b = randint(-10, 10)
> 
> print(f"{a}|{b}|{a + b}")
> ```
