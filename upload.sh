#!/bin/bash

programmer=/dev/usb/hiddev1
dir="$(dirname $0)"
exec "$dir"/arduwrap.sh -v --upload "$dir"/sketch/sketch.ino
#exec op -d ATtiny84 -p "$programmer" -w "$dir"/build/sketch.cpp.hex  -fuse 0xe2 -fuseh 0xdf -fusex 0xff
