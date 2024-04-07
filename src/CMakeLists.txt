add_executable(${NAME}
    main.cpp
    WifiHelper.cpp
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../secrets
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