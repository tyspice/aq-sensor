cmake_minimum_required(VERSION 3.13)

SET(NAME aq-sensor)
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/FreeRTOS-Kernel)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(${NAME})
set(PICO_BOARD pico_w)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${NAME}
    src/main.cpp
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/secrets
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${NAME}
    pico_stdlib
    hardware_gpio
	FreeRTOS-Kernel-Heap4
    pico_cyw43_arch_lwip_sys_freertos
    pico_lwip_mqtt
)


# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${NAME})