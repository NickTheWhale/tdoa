@echo off
cd /D C:\Users\nicholas\repos\tdoa\application\tag\zephyr\kconfig || (set FAIL_LINE=2& goto :ABORT)
C:\ncs\toolchains\c57af46cb7\opt\bin\cmake.exe -E env ZEPHYR_BASE=C:/Users/nicholas/repos/tdoa/external/zephyr PYTHON_EXECUTABLE=C:/ncs/toolchains/c57af46cb7/opt/bin/python.exe srctree=C:/Users/nicholas/repos/tdoa/external/zephyr KERNELVERSION=0x3036300 APPVERSION= CONFIG_=CONFIG_ KCONFIG_CONFIG=C:/Users/nicholas/repos/tdoa/application/tag/zephyr/.config ARCH=arm ARCH_DIR=C:/Users/nicholas/repos/tdoa/external/zephyr/arch BOARD_DIR=C:/Users/nicholas/repos/tdoa/external/zephyr/boards/arm/decawave_dwm1001_dev BOARD_REVISION= KCONFIG_BINARY_DIR=C:/Users/nicholas/repos/tdoa/application/tag/Kconfig APPLICATION_SOURCE_DIR=C:/Users/nicholas/repos/tdoa/application ZEPHYR_TOOLCHAIN_VARIANT=zephyr TOOLCHAIN_KCONFIG_DIR=C:/ncs/toolchains/c57af46cb7/opt/zephyr-sdk/cmake/zephyr TOOLCHAIN_HAS_NEWLIB=y EDT_PICKLE=C:/Users/nicholas/repos/tdoa/application/tag/zephyr/edt.pickle NCS_MEMFAULT_FIRMWARE_SDK_KCONFIG=C:/Users/nicholas/repos/tdoa/external/nrf/modules/memfault-firmware-sdk/Kconfig ZEPHYR_NRF_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/nrf ZEPHYR_HOSTAP_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/hostap ZEPHYR_MCUBOOT_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/bootloader/mcuboot ZEPHYR_MCUBOOT_KCONFIG=C:/Users/nicholas/repos/tdoa/external/nrf/modules/mcuboot/Kconfig ZEPHYR_MBEDTLS_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/crypto/mbedtls ZEPHYR_MBEDTLS_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/mbedtls/Kconfig ZEPHYR_TRUSTED_FIRMWARE_M_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/tee/tf-m/trusted-firmware-m ZEPHYR_TRUSTED_FIRMWARE_M_KCONFIG=C:/Users/nicholas/repos/tdoa/external/nrf/modules/trusted-firmware-m/Kconfig ZEPHYR_CJSON_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/cjson ZEPHYR_CJSON_KCONFIG=C:/Users/nicholas/repos/tdoa/external/nrf/modules/cjson/Kconfig ZEPHYR_AZURE_SDK_FOR_C_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/azure-sdk-for-c ZEPHYR_AZURE_SDK_FOR_C_KCONFIG=C:/Users/nicholas/repos/tdoa/external/nrf/modules/azure-sdk-for-c/Kconfig ZEPHYR_CIRRUS_LOGIC_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/hal/cirrus-logic ZEPHYR_OPENTHREAD_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/openthread ZEPHYR_OPENTHREAD_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/openthread/Kconfig ZEPHYR_MEMFAULT_FIRMWARE_SDK_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/memfault-firmware-sdk ZEPHYR_CANOPENNODE_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/canopennode ZEPHYR_CANOPENNODE_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/canopennode/Kconfig ZEPHYR_CHRE_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/chre ZEPHYR_CMSIS_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/hal/cmsis ZEPHYR_FATFS_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/fs/fatfs ZEPHYR_FATFS_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/fatfs/Kconfig ZEPHYR_HAL_NORDIC_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/hal/nordic ZEPHYR_HAL_NORDIC_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/hal_nordic/Kconfig ZEPHYR_ST_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/hal/st ZEPHYR_HAL_WURTHELEKTRONIK_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/hal/wurthelektronik ZEPHYR_LIBMETAL_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/hal/libmetal ZEPHYR_LIBLC3_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/liblc3 ZEPHYR_LIBLC3_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/liblc3/Kconfig ZEPHYR_LITTLEFS_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/fs/littlefs ZEPHYR_LITTLEFS_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/littlefs/Kconfig ZEPHYR_LORAMAC_NODE_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/loramac-node ZEPHYR_LORAMAC_NODE_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/loramac-node/Kconfig ZEPHYR_LVGL_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/gui/lvgl ZEPHYR_LZ4_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/lz4 ZEPHYR_LZ4_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/lz4/Kconfig ZEPHYR_MIPI_SYS_T_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/debug/mipi-sys-t ZEPHYR_NANOPB_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/nanopb ZEPHYR_NANOPB_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/nanopb/Kconfig ZEPHYR_NRF_HW_MODELS_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/bsim_hw_models/nrf_hw_models ZEPHYR_OPEN_AMP_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/open-amp ZEPHYR_PICOLIBC_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/picolibc ZEPHYR_SEGGER_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/debug/segger ZEPHYR_SEGGER_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/segger/Kconfig ZEPHYR_TINYCRYPT_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/crypto/tinycrypt ZEPHYR_TRACERECORDER_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/debug/TraceRecorder ZEPHYR_TRACERECORDER_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/TraceRecorder/Kconfig ZEPHYR_TF_M_TESTS_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/tee/tf-m/tf-m-tests ZEPHYR_PSA_ARCH_TESTS_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/tee/tf-m/psa-arch-tests ZEPHYR_UOSCORE_UEDHOC_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/uoscore-uedhoc ZEPHYR_UOSCORE_UEDHOC_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/uoscore-uedhoc/Kconfig ZEPHYR_ZCBOR_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/zcbor ZEPHYR_ZCBOR_KCONFIG=C:/Users/nicholas/repos/tdoa/external/zephyr/modules/zcbor/Kconfig ZEPHYR_ZSCILIB_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/zscilib ZEPHYR_NRFXLIB_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/nrfxlib ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR=C:/Users/nicholas/repos/tdoa/external/modules/lib/matter SHIELD_AS_LIST= DTS_POST_CPP=C:/Users/nicholas/repos/tdoa/application/tag/zephyr/zephyr.dts.pre DTS_ROOT_BINDINGS=C:/Users/nicholas/repos/tdoa/external/nrf/dts/bindings?C:/Users/nicholas/repos/tdoa/external/zephyr/dts/bindings C:/ncs/toolchains/c57af46cb7/opt/bin/python.exe C:/Users/nicholas/repos/tdoa/external/zephyr/scripts/kconfig/menuconfig.py C:/Users/nicholas/repos/tdoa/external/zephyr/Kconfig || (set FAIL_LINE=3& goto :ABORT)
goto :EOF

:ABORT
set ERROR_CODE=%ERRORLEVEL%
echo Batch file failed at line %FAIL_LINE% with errorcode %ERRORLEVEL%
exit /b %ERROR_CODE%