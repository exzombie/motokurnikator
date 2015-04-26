#!/bin/bash

dir="$(dirname $0)"

cd "$dir" && ./arduwrap.sh && \
    exec arduino --preferences-file "$dir"/preferences.txt "$dir"/sketch/sketch.ino
