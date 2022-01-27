#!/bin/bash

# build and run (to be invoked inside container)

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
bazel build //mud:main
bazel-bin/mud/main "$@"