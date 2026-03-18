import platform
from difflib import context_diff
from glob import glob
from os import chmod
from os.path import isfile
from subprocess import run
from sys import argv

SYSTEM = platform.system()

if SYSTEM == "Windows":
    EXECUTABLE = "cmake-build-debug/y64sim.exe"
    REFERENCE = "y64-ref/y64sim-win-x64.exe"
elif SYSTEM == "Darwin":  # macOS
    EXECUTABLE = "cmake-build-debug/y64sim"
    REFERENCE = "y64-ref/y64sim-mac-arm64"
    if isfile(REFERENCE):
        chmod(REFERENCE, 0o755)
elif SYSTEM == "Linux":
    EXECUTABLE = "cmake-build-debug/y64sim"
    REFERENCE = "y64-ref/y64sim-linux-x64"
    if isfile(REFERENCE):
        chmod(REFERENCE, 0o755)
else:
    assert False, f"Unsupported system: {SYSTEM}"

BINARY_PAT = "y64-*/*.bin"
MAX_STEPS = 10000


def main():
    if not isfile(EXECUTABLE):
        print(f"Executable {EXECUTABLE!r} not found")
        print(f"Please open {argv[0]} and edit EXECUTABLE path for {SYSTEM}")
        exit(1)

    binaries = glob(BINARY_PAT)
    if not binaries:
        print(f"Could not find binaries like {BINARY_PAT!r}")
        exit(1)

    if len(argv) > 1:
        name = argv[1]
        binaries = [binary for binary in binaries if name in binary]
        if not binaries:
            print(f"Could not find binaries like {name!r}")
            exit(1)

    binaries.sort()

    steps = int(argv[2]) if len(argv) > 2 else MAX_STEPS
    if steps != MAX_STEPS and not isfile(REFERENCE):
        print(f"Reference executable {REFERENCE!r} not found")
        exit(1)

    code = 0
    failed = 0
    for binary in binaries:
        returncode = test(binary, steps)
        if returncode:
            failed += 1
            code = returncode

    print(f"\n{len(binaries)} tests, {len(binaries) - failed} passed, {failed} failed")
    exit(code)


def test(binary: str, steps: int):
    print(f"[TEST] {binary}", end="")

    proc = run([EXECUTABLE, binary, str(steps)], capture_output=True, text=True, timeout=5)
    code = judge(binary, steps, proc.returncode, proc.stdout)

    if proc.stderr:
        print("stderr:")
        print(proc.stderr)

    return code


def judge(binary: str, steps: int, returncode: int, stdout_str: str):
    stdout = split_output(stdout_str)

    if returncode:
        print(f"\r[FAILED] {binary}: exit code {returncode}")
        if stdout:
            print("stdout:")
            print("".join(stdout))
        return returncode

    if steps != MAX_STEPS:
        ref = run([REFERENCE, binary, str(steps)], capture_output=True, text=True, timeout=5)
        assert ref.returncode == 0
        expected = split_output(ref.stdout)
    else:
        with open(binary.replace(".bin", ".out"), encoding="utf-8") as f:
            expected = split_output(f.read())

    if stdout == expected:
        print(f"\r[PASSED] {binary}")
        return 0
    else:
        print(f"\r[FAILED] {binary}: stdout mismatch")
        print("".join(context_diff(stdout, expected, "stdout", "expected")))
        return 1


def split_output(text: str):
    return [line.rstrip() + "\n" for line in text.rstrip().splitlines()]


if __name__ == "__main__":
    main()
