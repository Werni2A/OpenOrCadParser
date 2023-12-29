#!/usr/bin/env python3


import subprocess
import threading
from pathlib import Path

TEST_RUNNING_MSG = """
---------------------------------------------------------

Running Unit Tests...

---------------------------------------------------------
"""

TEST_SUCC_MSG = """
---------------------------------------------------------

Unit Tests Completed Successfully

---------------------------------------------------------
"""

TEST_FAIL_MSG = """
---------------------------------------------------------

Unit Tests Failed

---------------------------------------------------------
"""


lock = threading.Lock()
tests_ran_successfully = True


class TestThread(threading.Thread):
    def __init__(self, shard_count: int, shard_index: int):
        threading.Thread.__init__(self)
        self.shard_count = shard_count
        self.shard_index = shard_index

    def run(self):
        global tests_ran_successfully
        global lock

        lock.acquire()
        print(f"Starting shard {self.shard_index + 1} / {self.shard_count}")
        lock.release()

        result = subprocess.run(
            f"{test_file} --shard-count {self.shard_count} --shard-index {self.shard_index}",
            shell=True,
            capture_output=True,
            text=True,
        )

        lock.acquire()
        print(result.stdout)
        print(result.stderr)
        tests_ran_successfully = tests_ran_successfully and (result.returncode == 0)
        lock.release()


if __name__ == "__main__":
    lock_file = Path(__file__).parent / "test_err_cnt.log.lock"
    test_file = Path(__file__).parent / "build" / "test" / "test"

    if lock_file.exists():
        print(
            f"{lock_file} already exists. Probably from previously terminated test run."
        )
        print("Removing the lock file before continuing tests!")
        lock_file.unlink()

    print(TEST_RUNNING_MSG)

    threads = []

    TEST_SHARDS = 8
    for idx in range(TEST_SHARDS):
        threads += [TestThread(TEST_SHARDS, idx)]

    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()

    if tests_ran_successfully:
        print(TEST_SUCC_MSG)
        exit(0)
    else:
        print(TEST_FAIL_MSG)
        exit(1)
