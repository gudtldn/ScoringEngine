from enum import Enum


class UnsupportFileExtension(Exception):
    pass


class ESupportFileExtension(Enum):
    EXE = ".exe"
    PYTHON = ".py"


def insert_prefix(ext: str, cmd: list[str]):
    match ext:
        case ESupportFileExtension.EXE.value:
            pass

        case ESupportFileExtension.PYTHON.value:
            cmd.insert(0, "python")

        case _:
            raise UnsupportFileExtension(f"Unsupport file extension: {ext}")
