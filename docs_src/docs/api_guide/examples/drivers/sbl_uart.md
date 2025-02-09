# SBL UART {#EXAMPLES_DRIVERS_SBL_UART}

[TOC]

# Introduction

This bootloader does SOC initializations and attempts to boot a multicore appimage received over UART via XMODEM. The image file is sent using a python script (See \ref UART_BOOTLOADER_PYTHON_SCRIPT). Once image is received, the SBL then parses it, splits it into RPRCs for each core applicable. Each core is then initialized, RPRC image is loaded, entry points are set and the core is released from reset. For more on bootflow/bootloaders, please refer \ref BOOTFLOW_GUIDE

# Supported Combinations {#EXAMPLES_DRIVERS_SBL_UART_COMBOS}

\cond SOC_AM64X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER, @VAR_SK_BOARD_NAME_LOWER
 Example folder | examples/drivers/boot/sbl_uart

\endcond

\cond SOC_AM243X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/drivers/boot/sbl_uart

\endcond

\cond SOC_AM273X || SOC_AWR294X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/boot/sbl_uart

\endcond

\cond SOC_AM263X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/drivers/boot/sbl_uart

\endcond

\cond SOC_AM62X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER, @VAR_SK_LP_BOARD_NAME_LOWER, @VAR_SIP_SK_BOARD_NAME_LOWER
 Example folder | examples/drivers/boot/sbl_uart

\endcond

\cond SOC_AM62AX || SOC_AM62DX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/boot/sbl_uart

\endcond

\cond SOC_AM62PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | wkup-r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/boot/sbl_uart

\endcond

Refer \ref ENABLE_DDR_INLINE_ECC enablig inline ECC

# Steps to Run the Example
## Build the example
Since this is a bootloader and is used as a SOC initialization binary, the example will be run every time you boot an application using this example. It is generally run from a boot media (OSPI Flash, SD Card or over UART) unlike other examples which are usually loaded with CCS. Nevertheless, you can build this example like you do for the others using makefile or build it via CCS by importing as a project.

- **When using CCS projects to build**, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- **When using makefiles to build**, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE).

\cond SOC_AM62X || SOC_AM62AX || SOC_AM62DX || SOC_AM62PX

## Run the example
\cond !SOC_AM62PX
\note For HS-SE device, use default_sbl_uart_hs.cfg as the cfg file.
\endcond
\note For HS-FS device, use default_sbl_uart_hs_fs.cfg as the cfg file.

\cond SOC_AM62X
- Run the following command from the {MCU_PLUS_SDK_INSTALLATION_PATH}/tools/boot
   - For am62x-sk

            python uart_bootloader.py -p /dev/ttyUSB0 --cfg=sbl_prebuilt/am62x-sk/default_sbl_uart.cfg

   - For am62x-sk-lp

            python uart_bootloader.py -p /dev/ttyUSB0 --cfg=sbl_prebuilt/am62x-sk-lp/default_sbl_uart.cfg

   - For am62x-sip-sk

            python uart_bootloader.py -p /dev/ttyUSB0 --cfg=sbl_prebuilt/am62x-sip-sk/default_sbl_uart.cfg

\endcond

\cond SOC_AM62AX || SOC_AM62DX || SOC_AM62PX
- Run the following command from the {MCU_PLUS_SDK_INSTALLATION_PATH}/tools/boot
   - For @VAR_BOARD_NAME_LOWER

            python uart_bootloader.py -p /dev/ttyUSB0 --cfg=sbl_prebuilt/@VAR_BOARD_NAME_LOWER/default_sbl_uart.cfg

\endcond
Here /dev/ttyUSB0  can be changed to COM port like'COM4' incase of Windows.
\endcond
# See Also

\ref DRIVERS_BOOTLOADER_PAGE

# Sample Output

Since this SBL receives the appimage over UART using XMODEM protocol, it doesn't print anything to the console so as not to corrupt the XMODEM transport.
