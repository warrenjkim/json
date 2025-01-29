#!/usr/bin/env bash

docker build -t json-parser \
    --build-arg USER_NAME=$(whoami) \
    --build-arg USER_UID=$(id -u) \
    --build-arg USER_GID=$(id -g) .

docker run -it -v $(pwd):/workspace json-parser
