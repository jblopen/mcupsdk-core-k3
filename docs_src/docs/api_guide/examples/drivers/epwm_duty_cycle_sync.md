# EPWM Duty Cycle Sync {#EXAMPLES_DRIVERS_EPWM_DUTY_CYCLE_SYNC}

[TOC]

# Introduction

This example configures EPWM0/1/2 to generate symmetrical, Active-High
Complementary (AHC) outputs using EPWM channels A and B:
- EPWM0/1/2 are synchronized using the daisy-chain connectivity between the
EPWM modules. A Software Forced Synchronized Pulse is used at example
initialization to simulate a hardware synchronization signal.
- The Duty Cycle for each EPWM output signal is updated every EPWM period via
writes to the CMPA shadow registers in the EPWM ISR, where the Duty Cycle
values are proportional to sinusoidal values computed in the ISR.
- The EPWMs are configured in Up-Down count mode to generate symmetrical
outputs. The Deadband submodule is used to generate AHC outputs,
where the channel A input is the source for both the rising-edge and
falling-edge delays.

The following parameters are configurable at compile-time:
- EPWM output frequency: 4,8, or 16 kHz
- Initial Duty Cycle: 0.0 to 100.0 percent of EPWM period
- Time Base Phase on Sync In event: 0.0 to 100.0 percent of 1/2 EPWM period
- Deadband Rising Edge and Falling Edge delay
- Amplitude & frequency of sinusoids
- Duration of example execution

The following EPWM features are unused in this example:
- Chopper submodule
- Trip Zone submodule
- Event-Trigger submodule interrupt prescaling (every EPWM0 TBCNT=0
event triggers an interrupt)

\cond SOC_AM64X

An IO Breakout Board (BB) is required to probe the EPWM outputs. The table
below shows the jumper pins where the EPWM outputs can be observed.

A debug GPIO is driven in the EPWM ISR to show the EPWM period timing. The
GPIO output can be observed on the EVM J16.P1.

 EPWM   | EPWM Signal   | BB pin
 -------|---------------|-------
 0      | EPWM0_A       | J6.P7
 0      | EPWM0_B       | J6.P9
 1      | EPWM1_A       | J6.P11
 1      | EPWM1_B       | J6.P13
 2      | EPWM2_A       | J6.P17
 2      | EPWM2_B       | J6.P19

\endcond

\cond SOC_AM243X

An IO Breakout Board (BB) is required to probe the EPWM outputs. The table
below shows the jumper pins where the EPWM outputs can be observed.

A debug GPIO is driven in the EPWM ISR to show the EPWM period timing. The
GPIO output can be observed on the EVM J16.P1.

 EPWM   | EPWM Signal   | BB pin
 -------|---------------|-------
 0      | EPWM0_A       | J6.P7
 0      | EPWM0_B       | J6.P9
 1      | EPWM1_A       | J6.P11
 1      | EPWM1_B       | J6.P13
 2      | EPWM2_A       | J6.P17
 2      | EPWM2_B       | J6.P19

\endcond

\cond SOC_AM62AX || SOC_AM62DX

The table below shows the jumper pins where the EPWM outputs can be observed.

A debug GPIO is driven in the EPWM ISR to show the EPWM period timing. The
GPIO output can be observed on the base board J3 header.

 EPWM   | EPWM Signal   | Pin Details
 -------|---------------|-----------------------
 0      | EPWM0_A       | D16/GPIO1_15 (Pin_24)
 0      | EPWM0_B       | C16/GPIO1_16 (pin_26)
 1      | EPWM1_A       | B18/GPIO1_9  (Pin_36)
 1      | EPWM1_B       | B20/GPIO1_10 (pin_33)
 2      | EPWM2_A       | C17/GPIO1_28 (TP_49)
 2      | EPWM2_B       | E17/GPIO1_29 (TP_50)

\endcond

\cond SOC_AM62X
The table below shows the jumper pins where the EPWM outputs can be observed.

A debug GPIO is driven in the EPWM ISR to show the EPWM period timing. The
GPIO output can be observed on the pins (below mentioned) of User Expansion Connector (J3) on the  board.

## AM62X-SK

 EPWM   | EPWM Signal   | Pin Details
 -------|---------------|-----------------------
 0      | EPWM0_A       | A13/GPIO1_15 (Pin_24)
 0      | EPWM0_B       | C13/GPIO1_16 (pin_26)
 1      | EPWM1_A       | B18/GPIO1_9  (Pin_36)
 1      | EPWM1_B       | E18/GPIO1_10 (pin_33)

## AM62X-SK-LP

 EPWM   | EPWM Signal   | Pin Details
 -------|---------------|-----------------------
 0      | EPWM0_A       | C11/GPIO1_15 (Pin_24)
 0      | EPWM0_B       | D13/GPIO1_16 (pin_26)
 1      | EPWM1_A       | A18/GPIO1_9  (Pin_36)
 1      | EPWM1_B       | D18/GPIO1_10 (pin_33)

## AM62X-SK-SIP

 EPWM   | EPWM Signal   | Pin Details
 -------|---------------|-----------------------
 0      | EPWM0_A       | A13/GPIO1_15 (Pin_24)
 0      | EPWM0_B       | C13/GPIO1_16 (pin_26)
 1      | EPWM1_A       | B18/GPIO1_9  (Pin_36)
 1      | EPWM1_B       | E18/GPIO1_10 (pin_33)

\endcond

\cond SOC_AM62PX
The table below shows the jumper pins where the EPWM outputs can be observed.

A debug GPIO is driven in the EPWM ISR to show the EPWM period timing. The
GPIO output can be observed on the pins (below mentioned) of User Expansion Connector (J4) in the board.

 EPWM   | EPWM Signal   | Pin Details
 -------|---------------|-----------------------
 0      | EPWM0_A       | D20/GPIO1_15 (Pin_24)
 0      | EPWM0_B       | E20/GPIO1_16 (pin_26)
 1      | EPWM1_A       | E24/GPIO1_9  (Pin_36)
 1      | EPWM1_B       | F23/GPIO1_10 (pin_33)

\endcond

# Supported Combinations {#EXAMPLES_DRIVERS_EPWM_DUTY_CYCLE_SYNC_COMBOS}

\cond SOC_AM64X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/epwm/epwm_duty_cycle_sync/

\endcond

\cond SOC_AM243X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/epwm/epwm_duty_cycle_sync/

\endcond

\cond SOC_AM62AX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | a53ss0-0 nortos
 ^              | r5fss0-0 freertos
 ^              | c75ss0-0 freertos
 Toolchain      | arm.gnu.aarch64-none
 ^              | ti-arm-clang
 ^              | ti-c7000
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/epwm/epwm_duty_cycle_sync/

\endcond

\cond SOC_AM62DX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | a53ss0-0 nortos
 ^              | a53ss0-0 freertos
 ^              | c75ss0-0 freertos
 Toolchain      | arm.gnu.aarch64-none
 ^              | ti-c7000
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/epwm/epwm_duty_cycle_sync/

\endcond

\cond SOC_AM62X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | m4fss0-0 freertos
 ^              | a53ss0-0 freertos
 Toolchain      | ti-arm-clang
 ^              | arm.gnu.aarch64-none
 Board          | @VAR_BOARD_NAME_LOWER, @VAR_SK_LP_BOARD_NAME_LOWER, @VAR_SIP_SK_BOARD_NAME_LOWER
 Example folder | examples/drivers/epwm/epwm_duty_cycle_sync/

\endcond

\cond SOC_AM62PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | mcu-r5fss0-0 freertos
  ^             | wkup-r5fss0-0 freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/epwm/epwm_duty_cycle_sync/

\endcond

# Steps to Run the Example

- **When using CCS projects to build**, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- **When using makefiles to build**, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)
- Launch a CCS debug session and run the executable, see \ref CCS_LAUNCH_PAGE
- To probe the ePWM output please refer setup details as mentioned above in Introduction section

# See Also

\ref DRIVERS_EPWM_PAGE

# Sample Output

Shown below is a sample output when the application is run.

\code
EPWM Duty Cycle Sync Test Started ...
Please refer to the EXAMPLES_DRIVERS_EPWM_DUTY_CYCLE_SYNC example user guide for the test setup to probe the EPWM signals.
App will wait for 60 seconds (using PWM period ISR) ...
EPWM Duty Cycle Sync Test Passed!!
All tests have passed!!
\endcode
