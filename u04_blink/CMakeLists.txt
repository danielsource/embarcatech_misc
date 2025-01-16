# XXX: Pass -DPICO_BOARD=pico_w to cmake to specify Pico W.

cmake_minimum_required(VERSION 3.13)

set(PICO_PLATFORM "rp2040")
if (DEFINED ENV{PICO_SDK_PATH})
	set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
	message("Using PICO_SDK_PATH from environment ('${PICO_SDK_PATH}'")
else ()
        message(FATAL_ERROR "SDK location was not specified. Please set PICO_SDK_PATH.")
endif ()

set(PICO_SDK_INIT_CMAKE_FILE ${PICO_SDK_PATH}/pico_sdk_init.cmake)
include(${PICO_SDK_INIT_CMAKE_FILE})

project(rp2040_blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(blink rp2040_blink.c)
target_link_libraries(blink pico_stdlib)
if (PICO_CYW43_SUPPORTED)
	message("Pico has Wi-Fi.")
	target_link_libraries(blink pico_cyw43_arch_none)
endif ()

pico_enable_stdio_usb(blink 1)
pico_add_extra_outputs(blink)
