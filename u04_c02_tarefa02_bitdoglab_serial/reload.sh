#!/bin/sh

set -e
make -C build
picotool load -f build/bitdoglab_serial.uf2
