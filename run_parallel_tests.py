

import os
import sys
from asyncio.subprocess import Process
from io import TextIOWrapper
from pathlib import Path
from subprocess import Popen
from typing import List


if __name__ == "__main__":

    test_bin = Path(os.path.join(".", "build", "test", "test")).resolve()
    print(f"test_bin = {test_bin}")


    print()
    print("---------------------------------------------------------")
    print()
    print("Running Unit Tests...")
    print()
    print("---------------------------------------------------------")
    print()

    success = True
    shard_cnt = 8
    procs: List[Process] = []
    log_files: List[TextIOWrapper] = []

    for i in range(shard_cnt):

        f = open(f"test{i}.log", "w+")
        log_files += [f]
        cmd = [str(test_bin), "--shard-count", str(shard_cnt), "--shard-index", str(i)]
        procs += [Popen(cmd, stdout=f, stderr=f)]

    for proc in procs:
        proc.wait()
        success &= (proc.returncode == 0)

    for f in log_files:
        f.close()

    if success:
        print()
        print("---------------------------------------------------------")
        print()
        print("Unit Tests Completed Successfully")
        print()
        print("---------------------------------------------------------")
        print()

        sys.exit(0)
    else:
        print()
        print("---------------------------------------------------------")
        print()
        print("Unit Tests Failed")
        print()
        print("---------------------------------------------------------")
        print()

        sys.exit(1)
