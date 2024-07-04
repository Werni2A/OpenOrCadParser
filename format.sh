#!/bin/bash

REPO_DIR=$(realpath $(dirname $0))

find "$REPO_DIR/src" -iname "*.[ch]pp" -exec clang-format-15 --style=file:"$REPO_DIR/.clang-format" -i {} \;
