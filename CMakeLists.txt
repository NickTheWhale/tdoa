cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(tdoa)

target_sources(app PRIVATE
    src/cli.c
    src/config.c
    src/main.c
    src/uwb_anchor.c
    src/uwb_dummy.c
    src/uwb_tag.c
    src/uwb_utils.c
    src/uwb.c

    dw1000/src/deca_device.c
    dw1000/src/deca_mutex.c
    dw1000/src/deca_params_init.c
    dw1000/src/deca_range_tables.c
    dw1000/src/deca_sleep.c
    dw1000/src/deca_spi.c
    dw1000/src/port.c
)

target_include_directories(app PRIVATE
    dw1000/include
    include
)

target_compile_definitions(app PRIVATE
    DWT_API_ERROR_CHECK
)
