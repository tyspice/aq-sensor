add_executable(${NAME}
    main.cpp
    WifiHelper.cpp
    MQTTClient.cpp
    TimeHelper.cpp
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../config
    ${CMAKE_CURRENT_LIST_DIR}/../lib
    ${CMAKE_CURRENT_LIST_DIR}/../secrets
)

file(GLOB BME68X_SRC "../lib/bme68x/*.c")
add_library(bme68xLib ${BME68X_SRC})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${NAME}
    pico_stdlib
    hardware_gpio
    hardware_i2c
    hardware_rtc
	FreeRTOS-Kernel-Heap4
    pico_cyw43_arch_lwip_sys_freertos
    pico_lwip_mqtt
    pico_lwip_sntp
    bme68xLib
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${NAME})