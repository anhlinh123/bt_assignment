#!/bin/bash

set -e

TAG=booking:0.1

docker build . -t $TAG
docker run \
    --rm -it \
    -v $(pwd):/bt \
    $TAG bash
