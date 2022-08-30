import multiprocessing
import os
import sys
from asyncio.subprocess import Process
from pathlib import Path
from subprocess import Popen
from typing import List


if __name__ == "__main__":

    test_bin = Path(os.path.join(".", "build", "test", "test")).resolve()

    shard_cnt = multiprocessing.cpu_count()

    print()
    print("---------------------------------------------------------")
    print()
    print("Running Unit Tests...")
    print()
    print("---------------------------------------------------------")
    print()

    success = True
    procs: List[Process] = []

    print(f"Using {shard_cnt} Catch2 shards\n")

    for i in range(shard_cnt):
        cmd = [str(test_bin), "--shard-count", str(shard_cnt), "--shard-index", str(i)]
        procs += [Popen(cmd, shell=True)]

    for proc in procs:
        proc.wait()
        success &= (proc.returncode == 0)

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
