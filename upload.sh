#!/bin/bash

programmer=/dev/usb/hiddev1
dir="$(dirname $0)"
exec "$dir"/arduwrap.sh -v --upload "$dir"/sketch/sketch.ino
#exec op -d ATtiny84 -p "$programmer" -w "$dir"/build/sketch.cpp.hex 
