cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

set(PICO_BOARD pico_w)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

project(lwip)

add_executable(lwip
    src/main.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(lwip pico_stdlib pico_cyw43_arch_lwip_threadsafe_background pico_lwip_mqtt)

target_include_directories(lwip PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/secrets)


# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(lwip)