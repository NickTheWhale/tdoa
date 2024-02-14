cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(tdoa)

target_sources(app PRIVATE 
    src/main.c
    
    dw1000/src/deca_device.c
    dw1000/src/deca_mutex.c
    dw1000/src/deca_params_init.c
    dw1000/src/deca_range_tables.c
    dw1000/src/deca_sleep.c
    dw1000/src/deca_spi.c
    dw1000/src/port.c
    src/uwb.c
)

if (DEFINED APP_MODE AND APP_MODE STREQUAL "TAG")
    target_compile_definitions(app PRIVATE APP_MODE=0)
elseif (DEFINED APP_MODE AND APP_MODE STREQUAL "ANCHOR")
    target_compile_definitions(app PRIVATE APP_MODE=1)
endif()

target_include_directories(app PRIVATE 
    dw1000/include
    include
)