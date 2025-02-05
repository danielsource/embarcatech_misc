#!/bin/sh

set -e
make -C build
picotool load -f build/oled_i2c.uf2
