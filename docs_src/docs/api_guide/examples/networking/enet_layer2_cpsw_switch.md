# Enet Layer 2 CPSW SWITCH Example {#EXAMPLES_ENET_LAYER2_CPSW_SWITCH}

[TOC]

# Introduction

The layer 2 cpsw switch example is dedicated to demonstrate usage of Enet CPSW3G peripheral operation as a basic switch.

\cond SOC_AM62PX || SOC_AM62DX

On @VAR_SOC_NAME, we can do ethernet based communication using CPSW HW mechanism

- This is a standard ethernet switch + port HW

\endcond

This example do below:
- Target-side application running on a Cortex R5F core.
	- Target-side application running on a Cortex R5F core.
	- Application receives the broadcast packet for switch operation, copies the payload into a new packet which is then sent back out to the source port as well as to all the other ports on the device.
	- The application has a menu to enable/disable features, such as getting mac address and stats. This menu along with application logs are implemented via UART.
- Host-side functionality
	- Software applications like Colasoft Pkt Builder or packETH tool could be used to generate and send packets, Wireshark can be used to receive and verify packet contents

- The data path enabled in this example is as follows:
	- Host side (PC) application sends a broadcast packet to MAC port.
	- Target side application receives the broadcast packet, updates the MAC addresses in the Layer-2 header and sends the packet out back out to the source port as well as to all the other ports on the device.
	- Application like Wireshark (PC) receives the packet and can be seen in the capture window.

\cond SOC_AM64X

\attention

- AM64X-SK CPSW3G RGMII1 RX pins use IOset2 pinmux combination.
- The phy address of CPSW ethernet PHY-2 is 0x0001 for AM64X-SK.
- Use the default value in the AM64X-SK system configuration file for RX pins and phy addresses.


\endcond

\cond SOC_AM62PX || SOC_AM62DX

### Cut-thru Switching

- Cut Through feature allows forwarding packet from one external port to another without being stored in Port FIFOs thus reducing overall latency for packet forwarding.

#### Limitations:

- Cut Through forwarding is only supported between external ports and no support for Cut Through to host port currently.
- Cut-Thru is not supported with 10/100 half-duplex.
- Cut-Thru is not supported with any form of flow control.
- If Intersperced Express Traffic (IET) is enabled, then Cut Through can only be enabled on an express priority queue and not on preemptible queues.

#### Steps to enable Cut-thru switching:

- Set cutThruEnable Control register.
- Set rxPriCutThruEn(RX priority queue) and txPriCutThruEn(TX priority queue) which represents the priorties queues for which cut through feature needs to be enabled.
- Set portSpeedAutoEn for automatic detection of speed.
- Set CPSW frequency in Mhz which is used in auto speed detection for cut-thru operations

\endcond

# Supported Combinations

\cond SOC_AM62PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | wkup-r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/enet_layer2_cpsw_switch/V0

\endcond

\cond SOC_AM62DX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | mcu-r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/enet_layer2_cpsw_switch/V0

\endcond

# Steps to Run the Example

## Build the example

- When using CCS projects to build, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- When using makefiles to build, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)

## HW Setup

\note Make sure you have setup the EVM with cable connections as shown here, \ref EVM_SETUP_PAGE.
      In addition do below steps.
      Since the example runs on wkup R5, we cannot use CCS loading. Use example flashing methods shown in \ref EVM_SETUP_PAGE.

\cond SOC_AM62PX

### AM62PX-SK

#### For CPSW based example

- Connect two ethernet cables to the CPSW RGMII 1 and CPSW RGMII 2 ports in AM64X-SK from host PC as shown below

  \imageStyle{am62px_sk_cpsw_example.jpg,width:40%}
  \image html am62px_sk_cpsw_example.jpg Ethernet cable for CPSW based ethernet

\endcond

## Run the example

\attention If you need to reload and run again, a CPU power-cycle is MUST

- Launch a CCS debug session and run the example executable, see \ref CCS_LAUNCH_PAGE
- You will see logs in the UART terminal as shown in the next section.
- Launch a CCS debug session and run the example executable, see \ref CCS_LAUNCH_PAGE
- You will see logs in the UART terminal as shown in the next section.
- We can start sending multicast packets from Colasoft Pkt Builder or packETH tool.
  - In the Colasoft Pkt Builder, click on Add icon , select any layer2 Packet and click ok.
  - Edit the source and destination address in the decode editor.
  - Click on send icon and select the ethernet adapater.
  - Click start, packets will be send to the target.
- Capture the packets in Wireshark.

  \imageStyle{layer2_cpsw_example_packet_builder.png,width:60%}
  \image html layer2_cpsw_example_packet_builder.png Colasoft Pkt Builder to generate and send packets.

## Sample output for Multiport example

\code

==========================
 Layer 2 CPSW SWITCH Test 
==========================

Init all peripheral clocks
----------------------------------------------
Enabling clocks!

Create RX tasks
----------------------------------------------
cpsw-3g: Create RX task

Open all peripherals
----------------------------------------------
cpsw-3g: Open enet
EnetAppUtils_reduceCoreMacAllocation: Reduced Mac Address Allocation for CoreId:1 From 4 To 1 

Init all configs
----------------------------------------------
cpsw-3g: init config
cpsw-3g: Open port 1
EnetPhy_bindDriver:1842 
cpsw-3g: Open port 2
EnetPhy_bindDriver:1842 
PHY 0 is alive
PHY 1 is alive

Attach core id 1 on all peripherals
----------------------------------------------
cpsw-3g: Attach core
cpsw-3g: Open DMA
initQs() txFreePktInfoQ initialized with 16 pkts
cpsw-3g: Waiting for link up...
Cpsw_handleLinkUp:1450 
MAC Port 2: link up
Cpsw_handleLinkUp:1450 
MAC Port 1: link up
cpsw-3g: Port 1 link is up
cpsw-3g: Port 2 link is up
cpsw-3g: MAC port addr: 58:a1:5f:b8:8c:e6

Enet L2 cpsw Menu:
 's'  -  Print statistics
 'r'  -  Reset statistics
 'm'  -  Show allocated MAC addresses
 'p'  -  Enable Policer for rate limiting
 'x'  -  Stop the test

s

Print statistics
----------------------------------------------

 cpsw-3g - Port 1 statistics
--------------------------------


 cpsw-3g - Port 2 statistics
--------------------------------

p

Rate limiting Enabled port 1 on Src MAC 02:00:00:00:00:08
s

Print statistics
----------------------------------------------
  rxGoodFrames            = 100000
  rxOctets                = 101800000
  txGoodFrames            = 100000
  txBcastFrames           = 100000
  txOctets                = 101800000
  octetsFrames512to1023   = 200000
  netOctets               = 203600000
  txPri[0]                = 100000
  txPriBcnt[0]            = 101800000

 cpsw-3g - Port 1 statistics
--------------------------------
  txGoodFrames            = 150000
  txBcastFrames           = 100000
  txOctets                = 152700000
  octetsFrames512to1023   = 150000
  netOctets               = 152700000
  txPri[0]                = 150000
  txPriBcnt[0]            = 152700000

  rxGoodFrames            = 100000
  rxOctets                = 101800000
  txGoodFrames            = 100000
  txBcastFrames           = 100000
  txOctets                = 101800000
  octetsFrames512to1023   = 200000
  netOctets               = 203600000
  txPri[0]                = 100000
  txPriBcnt[0]            = 101800000

 cpsw-3g - Port 2 statistics
--------------------------------
  rxGoodFrames            = 100000
  rxBcastFrames           = 100000
  rxOctets                = 101800000
  txGoodFrames            = 50000
  txOctets                = 50900000
  octetsFrames512to1023   = 150000
  netOctets               = 152700000
  aleUnknownBcast         = 1
  aleUnknownBcastBcnt     = 1018
  txPri[0]                = 50000
  txPriBcnt[0]            = 50900000
\endcode

- On Wireshark we can see the packets received:

  \imageStyle{layer2_cpsw_example_wireshark_log.png,width:50%}
  \image html layer2_cpsw_example_wireshark_log.png Wireshark log for Layer 2 CPSW Example

## Testing Switch Policer configuration

- Enable the policer using 'p' option in target UART terminal menu option.
- Send Layer-2 packets with below configurations
    - source MAC address - 02:00:00:00:00:08
    - ingress port - PORT 1
    - Destination MAC address - any
    - Ether_type = 0x8600
    - Data rate - more than 25 mbps

- Observe that packets are switched by limiting data rate to 25 mpbs.
- Observe that the dropped packets are marked with 'alePolicyMatchRed' in CPSW statistics print.

# See Also

\ref NETWORKING
