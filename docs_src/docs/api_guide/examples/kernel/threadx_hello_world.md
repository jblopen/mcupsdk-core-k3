# ThreadX Hello World Example {#EXAMPLES_KERNEL_THREADX_HELLO_WORLD}

[TOC]

# Introduction

This example serves as a basic starting point for application development with the ThreadX kernel. The message "Hello world!" is printed on the CCS console as well as on the first available virtual COM port every one second.

# Supported Combinations {#EXAMPLES_KERNEL_THREADX_HELLO_WORLD_COMBOS}

\cond SOC_AM62AX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | a53ss0-0 threadx
 ^              | mcu-r5fss0-0 threadx
 Toolchain      | arm.gnu.aarch64-none
 ^              | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | examples/kernel/threadx/hello_world

\endcond

# Steps to Run the Example

- **When using CCS projects to build**, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- **When using makefiles to build**, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)
- Launch a CCS debug session and run the executable, see \ref CCS_LAUNCH_PAGE

# See Also

\ref KERNEL_THREADX

# Sample Output

\code
[FILEX HELLO WORLD] Hello world example started...
Hello world!
Hello world!
Hello world!
\endcode