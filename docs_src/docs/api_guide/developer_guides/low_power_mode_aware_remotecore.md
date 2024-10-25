# Low power mode implementation of remote cores {#LOW_POWER_MODE_AWARE_REMOTECORE}

[TOC]

## Introduction

\cond SOC_AM62X
Linux running on A53 core can load the firmware to the remote cores. MCU M4F core incase of AM62X. Refer \htmllink{https://dev.ti.com/tirex/explore/node?node=A__AISILbMWZ4d84U2oSmXcdA__linux_academy_am62x__XaWts8R__LATEST, Linux Academy for AM62X} for more details on how to boot the remotecores. This section explains how to add support for low power mode aware firmware on the remotecore.
\endcond


\cond SOC_AM62AX
Linux running on A53 core can load the firmware to the remote cores. MCU R5F/ C7X core incase of AM62AX. Refer \htmllink{https://dev.ti.com/tirex/explore/node?node=A__AS4TbXvv.-OIrnxuMySmZg__AM62A-ACADEMY__WeZ9SsL__LATEST, AM62Ax Academy} for more details on how to boot the remotecores. This section explains how to add support for low power mode aware firmware on the remotecore.
\endcond


\cond SOC_AM62PX
Linux running on A53 core can load the firmware to the remote cores. MCU R5F core incase of AM62PX. This section explains how to add support for low power mode aware firmware on the remotecore.
\endcond

## Implementing low power mode aware remotecore firmware

 - Suspend task is created to enable graceful suspend in low power mode.
 - An IPC message is sent to remotecore by Remoteproc driver running on linux to query about the next state that core is going to enter.

 - To receive and handle IPC messages, register a callback as shown below.

````C
    /* Register a callback for the RP_MBOX messages from the Linux remoteproc driver*/
    IpcNotify_registerClient(IPC_NOTIFY_CLIENT_ID_RP_MBOX, &ipc_rp_mbox_callback, NULL);
````
 - On the callback, post the suspend thread.
````C
volatile uint8_t gbSuspended = 0u;
volatile uint8_t gbSuspendRemotecoreID = 0u;
void ipc_rp_mbox_callback(uint16_t remoteCoreId, uint16_t clientId, uint32_t msgValue, void *args)
{
    if (clientId == IPC_NOTIFY_CLIENT_ID_RP_MBOX)
    {
        if (msgValue == IPC_NOTIFY_RP_MBOX_SUSPEND_SYSTEM) /* Suspend request received from linux during LPM suspend */
        {
            gbSuspendRemotecoreID = remoteCoreId;
            SemaphoreP_post(&gLpmSuspendSem);
        }
    }
}
````
 - On the suspend thread, query the next state from device manager.

 ````C
    uint8_t nextHostState;

    /* Wait for suspend from linux */
    SemaphoreP_pend(&gLpmSuspendSem, SystemP_WAIT_FOREVER);

    status = Sciclient_lpmGetNextHostState(SystemP_WAIT_FOREVER, &nextHostState);
    if (status != SystemP_SUCCESS)
    {
        DebugP_log("[IPC RPMSG ECHO] Failed to get next system state. Canceling suspend.\r\n");
        IpcNotify_sendMsg(gbSuspendRemotecoreID, IPC_NOTIFY_CLIENT_ID_RP_MBOX, IPC_NOTIFY_RP_MBOX_SUSPEND_CANCEL, 1u);
        continue;
    }

    DebugP_log("[IPC RPMSG ECHO] Next MCU mode is %d\r\n", nextHostState);
````
 - The response can be OFF or ON.

### ON - MCU only LPM
\attention This Low power mode is supported only by MCU remote cores.

 - ON state will be responded. As the core has to stay ON, send SUSPEND_AUTO to remoteproc driver.

````C
    gbSuspended = 1u;

    /* Print before sending ACK, otherwise IO isolation is enabled while printing */
    DebugP_log("[IPC RPMSG ECHO] Suspend request to MCU-only mode received \r\n");
    DebugP_log("[IPC RPMSG ECHO] Press a single key on this terminal to resume the kernel from MCU only mode \r\n");

    IpcNotify_sendMsg(gbSuspendRemotecoreID, IPC_NOTIFY_CLIENT_ID_RP_MBOX, IPC_NOTIFY_RP_MBOX_SUSPEND_AUTO, 1u);
    lpm_mcu_wait_for_uart();
    gbSuspended = 0u;
````
 - MCU core then waits for key to be pressed at MCU UART. UART callback has to be enabled for resume support from MCU UART.

 - Wait for key to be pressed on MCU UART / any other system interrupt.

````C
    while (gNumBytesRead == 0u && gbSuspended == 1u)
    {
    }
````
#### Wakeup from MCU UART

 - If the key is pressed on MCU UART, the callback sets gNumBytesRead.
 - The remotecore performs IPC to trigger system resume.

````C
    if (gNumBytesRead != 0)
    {
        DebugP_log("[IPC RPMSG ECHO] Key pressed. Notifying DM to wakeup main domain\r\n");
        SOC_triggerMcuLpmWakeup();
        /* Wait for resuming the main domain */
        SemaphoreP_pend(&gLpmResumeSem, SystemP_WAIT_FOREVER);

        DebugP_log("[IPC RPMSG ECHO] Main domain resumed due to MCU UART \r\n");
    }
````
 - The linux resumes and remoteproc sends ECHO_REQUEST message to remote core.
 - On the callback, post the resume thread.

````C
    void ipc_rp_mbox_callback(uint16_t remoteCoreId, uint16_t clientId, uint32_t msgValue, void *args)
    {
        if (clientId == IPC_NOTIFY_CLIENT_ID_RP_MBOX)
        {
            if (msgValue == IPC_NOTIFY_RP_MBOX_ECHO_REQUEST) /* This message is received after resuming from the MCU only LPM. */
            {
                gbSuspended = 0u;

                if (gNumBytesRead != 0u)
                {
                    /* post this only for MCU UART Wakeup */
                    SemaphoreP_post(&gLpmResumeSem);
                }
                DebugP_shmLogWriterResume();

                IpcNotify_sendMsg(remoteCoreId, IPC_NOTIFY_CLIENT_ID_RP_MBOX, IPC_NOTIFY_RP_MBOX_ECHO_REPLY, 1u);
            }
        }
    }
````
- Remote core is out of low power mode.

#### Wakeup from any other wakeup source

 - The linux resumes and remoteproc sends ECHO_REQUEST message to remote core.
 - On the callback, clear the gbSuspended variable.
 - Cancel UART Read and remote core exits low power mode.

````C
    if (gbSuspended == 0u)
    {
        UART_readCancel(gUartHandle[CONFIG_UART0], &trans);
        DebugP_log("[IPC RPMSG ECHO] Main domain resumed from a different wakeup source \r\n");
    }
````

### OFF - Deep Sleep / IO Only plus DDR LPM
 - OFF state will be responded. As the core has to be shutdown, send SUSPEND_ACK to remoteproc driver to initiate shutdown of this core.

````C
    IpcNotify_sendMsg(gbSuspendRemotecoreID, IPC_NOTIFY_CLIENT_ID_RP_MBOX, IPC_NOTIFY_RP_MBOX_SUSPEND_ACK, 1u);
````

 - An IPC message is then sent to the remote core by remoteproc for graceful shutdown. Refer \ref GRACEFUL_REMOTECORE_SHUTDOWN

This is implemented on \ref EXAMPLES_DRIVERS_IPC_RPMESSAGE_LINUX_ECHO