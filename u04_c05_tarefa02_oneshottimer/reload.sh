#!/bin/sh

set -e
make -C build
picotool load -f build/oneshottimer.uf2
