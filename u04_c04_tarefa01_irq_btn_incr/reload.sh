#!/bin/sh

set -e
make -C build
picotool load -f build/btn_incr.uf2
