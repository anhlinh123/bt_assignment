#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

set -x

json_format() {
    for file in $@; do
        jq . ${file} > ${file}.tmp
        mv ${file}.tmp ${file}
    done
}

export -f json_format

find $SCRIPT_DIR \
    -path "*/CMakeFiles" -prune -type f \
    -o \( -name "*.cpp" -o -name "*.hpp" \) \
    -exec clang-format -i {} +
find $SCRIPT_DIR \
    -path "*/CMakeFiles" -prune -type f \
    -o -path "*/build" -prune -type f \
    -o -name "*.json" \
    -exec bash -c 'json_format $@' - {} +
