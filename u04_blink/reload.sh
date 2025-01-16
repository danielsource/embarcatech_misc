#!/bin/sh

set -e
make -C build
picotool load -f build/blink.uf2
