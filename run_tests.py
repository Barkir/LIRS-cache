import questionary
from pathlib import Path
import subprocess

IDEAL_DIR = Path("./tests/tests_ideal/")
IDEAL_RUN = Path("./build/ideal")

LIRS_DIR  = Path("./tests/tests_lirs/")
LIRS_RUN  = Path("./build/lirs")

LRU_DIR   = Path("./tests/tests_lru/")
LRU_RUN   = Path("./build/lru")

tests = questionary.select(
    "which tests would you like to run?",
    choices=["IDEAL", "LRU", "LIRS"]
    ).ask()


def testLIRS():
    for dat_file in sorted(LIRS_DIR.iterdir()):
        with open(dat_file, 'r') as f:
            result=subprocess.run(
                [LIRS_RUN],
                stdin=f,
                capture_output=True,
                text=True
                )
        print(result.stdout, end="")

def testIDEAL():
    for dat_file in sorted(IDEAL_DIR.iterdir()):
        with open(dat_file, 'r') as f:
            result=subprocess.run(
                [IDEAL_RUN],
                stdin=f,
                capture_output=True,
                text=True
                )
        print(result.stdout, end="")

def testLRU():
    for dat_file in sorted(LRU_DIR.iterdir()):
        with open(dat_file, 'r') as f:
            result=subprocess.run(
                [LRU_RUN],
                stdin=f,
                capture_output=True,
                text=True
                )
        print(result.stdout, end="")


if (tests == "LIRS"):
    testLIRS()
elif (tests == "LRU"):
    testLRU()
elif (tests == "IDEAL"):
    testIDEAL()
