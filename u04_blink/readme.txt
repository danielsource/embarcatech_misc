Build example on Linux
----------------------

  % cmake -DPICO_BOARD=pico_w -B build	# Assuming $PICO_SDK_PATH is installed
  % make -C build
  % picotool load build/blink.uf2	# Assuming BOOTSEL mode


Reading the serial output
-------------------------

  % screen /dev/ttyACM0 115200		# Assuming permission to ttyACM0


Recommended reading
-------------------

  - https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf
