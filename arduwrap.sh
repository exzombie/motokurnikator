#!/bin/bash

dir="$(dirname $0)"
[[ ! -d "$dir"/build ]] && mkdir "$dir"/build

exec arduino \
     --preferences-file "$dir"/preferences.txt \
     --pref sketchbook.path="$dir" \
     --pref build.path="$dir"/build \
     "$@"
