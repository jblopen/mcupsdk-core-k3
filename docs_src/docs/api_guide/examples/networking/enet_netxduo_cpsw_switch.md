# Enet NetxDuo CPSW Switch Example{#EXAMPLES_ENET_NETXDUO_CPSW_SWITCH}

[TOC]

# Introduction

\note NetxDuo features are made available as is from public NetxDuo project. SDK configuration may only enable and exercise a subset of these features.

This example is a TCP/UDP IP application using the NetxDuo networking stack, coupled with
ethernet driver (ENET)

On @VAR_SOC_NAME, we can do ethernet based communication using CPSW as HW mechanism
  - CPSW is a standard ethernet switch + port HW
  - It uses ethernet driver underneath with NetxDuo TCP/IP networking stack
  - CPSW is configured in Switch mode.

This example does the following
- Initializes the ethernet driver for the underlying HW
- Initializes the NetxDuo stack for TCP/UDP IP
- Allows user to run and test basic networking features like DHCP and ping with TCP/UDP.
- Is configured to run in Switch mode.

# Supported Combinations


 Parameter      | Value
 ---------------|-----------
 CPU + OS       | a53ss0-0 threadx
 Toolchain      | arm.gnu.aarch64-none
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | source/networking/enet/core/examples/netxduo/enet_netxduo_cpsw_switch

Note: To run the example on any core other than a53ss0-0, user needs to change the DMA channel resource ownership accordingly using the resource partioning tool in \ref RESOURCE_ALLOCATION_GUIDE and build the new SBL.

# Configuring Syscfg

- Following Syscfg option allows flexibility to configure memory foot print based on required use case like: Number of DMA descriptors and buffering.

- Supported Options with default configuration

<table>
<tr>
    <th>Feature
    <th>Section
    <th>Description
    <th>Remarks/Default Setting
</tr>

<tr>
    <td>Mdio Manual Mode Enable
    <td>TI Networking / Enet (CPSW)
    <td>Flag to enable MDIO manual mode in example. Driver support for Manual mode is enabled, so this parameter configures manual mode in the example.
    <td>Default is true. If your silicon is affected with errata <a href="https://www.ti.com/lit/er/sprz457e/sprz457e.pdf" target="_blank">i2329— MDIO interface corruption</a>, then TI suggests to use MDIO_MANUAL_MODE as software workaround.
</tr>

<tr>
    <td>Disable Mac Port1, Disable Mac Port2
    <td>TI Networking / Enet (CPSW)
    <td>Select which port to disable.
    <td>Default is Port1 enabled. If both Port1 and Port 2 are enabled, any port can be used and  if operating in switch mode, it enables traffic switching between the two ports.
</tr>

<tr>
    <td>Enable Packet Pool Allocation
    <td>TI Networking / Enet (CPSW)
    <td>Flag to enable packet buffer memory allocation from enet utils library. It should be disabled to avoid utils memory wastage, in case application allots packet via other mechanism.
    <td>Default is true. If enabled size of pkt pool size depends on 'Large Pool Packet Size', 'Large Pool Packet Count', 'Medium Pool Packet Size', 'Medium Pool Packet Count', 'Small Pool Packet Size' and 'Small Pool Packet Count'. EnetMem_allocEthPkt API uses this memory to allocate the DMA Ethernet packet.
</tr>

<tr>
    <td>Only Enable Packet Info Allocation
    <td>TI Networking / Enet (CPSW)
    <td>Flag to allocate only the DMA Packet Info structures, this does not include the buffer memory. This is useful when the buffer memory is internally allocated by the application.
    <td>Default is true. If enabled "PktInfoMem Only Count" determines the number of additional DMA Packet Info structures allocated. EnetMem_allocEthPktInfoMem uses this memory to allocate empty DMA Packet Info structures.
</tr>

<tr>
    <td>Number of Tx Packet
    <td>TI Networking / Enet (CPSW) / DMA channel config
    <td>No of Tx packets required for DMA channel
    <td>Default is 16. The size of the NetxDuo packet pool will be automatically set at runtime to match the number of packets configured here.
</tr>

<tr>
    <td>Number of Rx Packet
    <td>TI Networking / Enet (CPSW) / DMA channel config
    <td>No of Rx packets required for DMA channel
    <td>Default is 32. It contributes to the size of Pkt Mem Pool, DMA ring buffer and accessories size. The size of the NetxDuo packet pool will be automatically set at runtime to match the number of packets configured here.
</tr>

<tr>
    <td>NetxDuo instance
    <td>TI Networking / NetxDuo
    <td>No of interfaces allocated by the example
    <td>Only one interface should be set to default when more than one interface is allocated.
</tr>
</table>


# Steps to Run the Example

## Build the example

- When using CCS projects to build, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- When using makefiles to build, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)

## HW Setup

\note Make sure you have setup the EVM with cable connections as shown here, \ref EVM_SETUP_PAGE.
      In addition do below steps.



### AM62AX-SK

- Connect a ethernet cable to one of the ports on the EVM from host PC.

## Create a network between EVM and host PC

- The EVM will get an IP address using DHCP, so make sure to connect the other end of the cable
to a network which has a DHCP server running.

- To get started one can create a simple local network
  between the EVM and the host PC by using a home broadband/wifi router as shown below.
  Most such routers run a DHCP server

  \imageStyle{lwip_example_01.png,width:30%}
  \image html lwip_example_01.png Local network between PC and EVM

- To check the router connection with host PC, recommend to disconnect all other networking conenctions
  on the PC, sometimes you may need to disable firewall SW, and make sure the router is able
  to assign a IP address to your host PC

- After we run the example on the EVM (next step), the EVM will similarly be assigned a IP address, and then host
  can communicate with the EVM using the assigned IP address.

## Run the example

\attention If you need to reload and run again, a CPU power-cycle is MUST

- Launch a CCS debug session and run the example executable, see \ref CCS_LAUNCH_PAGE
- You will see logs in the UART terminal as shown in the next section.
- Note the IP address seen in the log, this is what we will use to communicate with the EVM.

## Sample output for CPSW example


\code
==============================
   NETXDUO CPSW SWITCH MODE   
==============================
Enabling clocks!
EnetAppUtils_reduceCoreMacAllocation: Reduced Mac Address Allocation for CoreId:2 From 4 To 2 
Open MAC port 1
EnetPhy_bindDriver: PHY 0: OUI:080028 Model:23 Ver:01 <-> 'DP83867' : OK

Open MAC port 2
EnetPhy_bindDriver: PHY 1: OUI:080028 Model:23 Ver:01 <-> 'DP83867' : OK

PHY 0 is alive
PHY 1 is alive
Cpsw_internalIoctl_handler_ENET_IOCTL_REGISTER_DSTMAC_RX_FLOW: CPSW: Registered MAC address.ALE entry:1, Policer Entry:0
Waiting for address from DHCP server on primary interface...
Cpsw_handleLinkUp: Port 1: Link up: 1-Gbps Full-Duplex

MAC Port 1: link up
Local Interface IP is: 192.168.1.100
\endcode


## Communicate with the EVM using ethernet

- You can try to reach the EVM using ping as shown below, using a command shell on the host PC

        > ping 192.168.1.100


## Troubleshooting issues


- If the execution hangs at `Waiting for address from DHCP server on primary interface...`
   - Make sure you see `MAC Port X: link up` message, if not check the ethernet cable
   - Check the local network and check if the DHCP server is indeed running as expected
   - When using a home broadband/wifi router, its possible to check the clients connected to the DHCP server via a web
     browser. Check your router user manual for more details.

# See Also
\ref NETWORKING