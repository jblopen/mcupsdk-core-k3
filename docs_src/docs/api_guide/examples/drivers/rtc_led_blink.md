# RTC LED Blink {#EXAMPLES_DRIVERS_RTC_LED_BLINK}

[TOC]

# Introduction

This example configures a GPIO pin connected to an LED on the EVM in output mode.
The application toggles the LED ON/OFF for 10 times using RTC Timer.

# Supported Combinations {#EXAMPLES_DRIVERS_RTC_LED_BLINK_COMBOS}

\cond SOC_AM62X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 freertos
 ^              | a53ss0-0 freertos
 Toolchain      | ti-arm-clang
 ^              | arm.gnu.aarch64-none
 Board          | @VAR_BOARD_NAME_LOWER, @VAR_SIP_SK_BOARD_NAME_LOWER
 Example folder | examples/drivers/rtc/rtc_led_blink

\endcond

# Steps to Run the Example

- **When using CCS projects to build**, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- **When using makefiles to build**, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)
\cond !SOC_AM62X
- Launch a CCS debug session and run the executable, see \ref CCS_LAUNCH_PAGE
\endcond
\cond SOC_AM62X
\attention As the wake-up R5 is the device manager, it needs to be started by the SBL. So it can not be loaded through CCS. It should be flashed and booted through SBL.

- Refer \ref GETTING_STARTED_FLASH for flashing the application.
\endcond
# See Also

\ref DRIVERS_RTC_PAGE

# Sample Output

\code
[RTC LED Blink Test] Starting ...
[RTC LED Blink Test] Started...
LED blinked successfully at time: 07/11/2024 14:45:12
LED blinked successfully at time: 07/11/2024 14:45:14
LED blinked successfully at time: 07/11/2024 14:45:16
LED blinked successfully at time: 07/11/2024 14:45:18
LED blinked successfully at time: 07/11/2024 14:45:20
LED blinked successfully at time: 07/11/2024 14:45:22
LED blinked successfully at time: 07/11/2024 14:45:24
LED blinked successfully at time: 07/11/2024 14:45:26
LED blinked successfully at time: 07/11/2024 14:45:28
LED blinked successfully at time: 07/11/2024 14:45:30
RTC LED blink test passed!!
\endcode
