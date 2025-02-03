cmake_minimum_required(VERSION 3.13)

set(PICO_PLATFORM "rp2040")
set(PICO_BOARD "pico_w")
if (DEFINED ENV{PICO_SDK_PATH})
	set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
	message("Using PICO_SDK_PATH from environment ('${PICO_SDK_PATH}')")
else ()
        message(FATAL_ERROR "SDK location was not specified. Please set PICO_SDK_PATH.")
endif ()
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(snake C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wno-parentheses)
add_executable(${PROJECT_NAME} src/snake.c)
target_link_libraries(${PROJECT_NAME}
	pico_stdlib
	hardware_pio)

pico_sdk_init()
pico_generate_pio_header(${PROJECT_NAME}
	${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio
	OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})
