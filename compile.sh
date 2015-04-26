#!/bin/bash

dir="$(dirname $0)"
exec "$dir"/arduwrap.sh -v --verify "$dir"/sketch/sketch.ino
