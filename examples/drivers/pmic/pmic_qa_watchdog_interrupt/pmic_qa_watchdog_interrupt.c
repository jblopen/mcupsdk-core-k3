/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file pmic_qa_watchdog_interrupt.c
 *
 *  \brief This is a PMIC watchdog example for Q&A mode which will generate
 *         a reset when MCU fails to send a signal to the watchdog.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <board/pmic.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/SemaphoreP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                   External Function Declarations                           */
/* ========================================================================== */

extern void Board_gpioIrqnit(void);
extern void Board_gpioIrqDeinit(void);
extern uint32_t Board_getGpioIntrNum(void);

/* ========================================================================== */
/*                   Internal Function Declarations                           */
/* ========================================================================== */

static int32_t PMICApp_setGPIOPins(PMIC_Config pmicConfig);
static int32_t PMICApp_gpioIntConfigure();
static void PMICApp_gpioIntDestruct();
static int32_t PMICApp_wdogQAModeInt(PMIC_Config pmicConfig);
static void PMICApp_checkForWdgErrors(Pmic_CoreHandle_t *coreHandle);
static void PMICApp_gpioISR(void *args);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* The address of GPIO instance used for receiving the interrupt from PMIC */
static uint32_t gGpioWdogIntBaseAddr = (uint32_t) (GPIO_WDOG_INT_BASE_ADDR);

/* Semaphore object to notify PMIC GPIO interrupt has occurred */
static SemaphoreP_Object gGpioISRDoneSem;

/* HwiP object for GPIO interrupt */
static HwiP_Object         gGpioHwiObject;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void pmic_qa_watchdog_interrupt_main(void *args)
{
    uint32_t status = SystemP_FAILURE;

    /* PMIC interface handle initialized by PMIC_open */
    PMIC_Config pmicConfig = gPmicConfig[CONFIG_PMIC0];
    DebugP_assert(NULL != pmicConfig.pmicCoreHandle);

    DebugP_log("\r\n");
    DebugP_log("Starting Q&A watchdog interrupt example !!\r\n");

    /* Set the PMIC GPIO pin for Q&A watchdog based on communication mode */
    status = PMICApp_setGPIOPins(pmicConfig);
    if(SystemP_SUCCESS == status)
    {
        /* Configure the GPIO pin to receive interrupt from PMIC */
        status = PMICApp_gpioIntConfigure();
    }

    if(SystemP_SUCCESS == status)
    {
        status = SemaphoreP_constructBinary(&gGpioISRDoneSem, 0);
        if(SystemP_SUCCESS == status)
        {
        /* Configure the PMIC to start watchdog Q&A mode and check
         * if interrupt is received when answers are not sent.
         */
            status = PMICApp_wdogQAModeInt(pmicConfig);
        }
    }

    if(SystemP_SUCCESS == status)
    {
        DebugP_log("All tests have passed!!\r\n");
    }
    else
    {
        DebugP_logError ("Some tests have failed!!\r\n");
    }

    PMICApp_gpioIntDestruct();

    return;
}

static int32_t PMICApp_setGPIOPins(PMIC_Config pmicConfig)
{
    int32_t        status = PMIC_ST_SUCCESS;
    int32_t        retVal = SystemP_FAILURE;
    Pmic_CoreHandle_t *coreHandle = pmicConfig.pmicCoreHandle;
    const Pmic_GpioCfg_t gpio1Cfg =
    {
        .validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT,
        .pinFunc = pmicConfig.qaWdogpin1Func,
    };
    const Pmic_GpioCfg_t gpio2Cfg =
    {
        .validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT,
        .pinFunc = pmicConfig.qaWdogpin2Func,
    };

    status = Pmic_gpioSetConfiguration(coreHandle, pmicConfig.qaWdogpin1, gpio1Cfg);
    if (status == PMIC_ST_SUCCESS)
    {
        status = Pmic_gpioSetConfiguration(coreHandle, pmicConfig.qaWdogpin2, gpio2Cfg);
    }

    if(status == PMIC_ST_SUCCESS)
    {
        DebugP_log("Configure PMIC GPIO pins for Q&A watchdog based on communication mode... DONE\r\n");
        retVal = SystemP_SUCCESS;
    }
    else
    {
        DebugP_logError("PMIC GPIO pins for Q&A watchdog based on communication mode ... Failed !!\r\n");
    }

    return retVal;
}

static int32_t PMICApp_gpioIntConfigure()
{
    int32_t         status = SystemP_FAILURE;
    uint32_t        pinNum, intrNum;
    uint32_t        bankNum;
    HwiP_Params     hwiPrms;

    Board_gpioIrqnit();

    pinNum          = GPIO_WDOG_INT_PIN;
    intrNum         = Board_getGpioIntrNum();
    bankNum         = GPIO_GET_BANK_INDEX(pinNum);

    /* Setup GPIO for interrupt generation */
    GPIO_setDirMode(gGpioWdogIntBaseAddr, pinNum, GPIO_WDOG_INT_DIR);
    GPIO_setTrigType(gGpioWdogIntBaseAddr, pinNum, GPIO_WDOG_INT_TRIG_TYPE);
    GPIO_bankIntrEnable(gGpioWdogIntBaseAddr, bankNum);

    /* Register GPIO interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum   = intrNum;
    hwiPrms.isPulse = 1;
    hwiPrms.callback = &PMICApp_gpioISR;
    hwiPrms.args     = (void *) pinNum;

    status = HwiP_construct(&gGpioHwiObject, &hwiPrms);
    if(status == SystemP_SUCCESS)
    {
        DebugP_log("Configure GPIO pin in the board to receive PMIC interrupt... DONE\r\n");
    }
    else
    {
        DebugP_logError("Configure GPIO pin in the board to receive PMIC interrupt... Failed !!\r\n");
    }

    return status;
}

static void PMICApp_gpioIntDestruct()
{
    uint32_t        pinNum, bankNum;

    pinNum          = GPIO_WDOG_INT_PIN;
    bankNum         = GPIO_GET_BANK_INDEX(pinNum);

    /* Unregister interrupt */
    GPIO_bankIntrDisable(gGpioWdogIntBaseAddr, bankNum);
    GPIO_setTrigType(gGpioWdogIntBaseAddr, pinNum, GPIO_TRIG_TYPE_NONE);
    GPIO_clearIntrStatus(gGpioWdogIntBaseAddr, pinNum);
    HwiP_destruct(&gGpioHwiObject);
    Board_gpioIrqDeinit();

    return;
}

static int32_t PMICApp_wdogQAModeInt(PMIC_Config pmicConfig)
{
    int32_t        status = PMIC_ST_SUCCESS;
    int32_t        retVal = SystemP_FAILURE;
    uint16_t      numSeqeunces = 0;
    uint8_t       answerCnt = 0;
    bool    wdgEnabled = false;
    uint8_t isWdogInt = false;
    Pmic_CoreHandle_t *coreHandle = pmicConfig.pmicCoreHandle;
    Pmic_WdgCfg_t tmpWdgCfg;
    Pmic_WdgErrStatus_t   wdgErrStat =
    {
        .validParams = PMIC_CFG_WD_FAIL_INT_ERRSTAT_VALID_SHIFT
    };

    status = Pmic_wdgGetEnableState(coreHandle, &wdgEnabled);
    if (status == PMIC_ST_SUCCESS && !wdgEnabled)
    {
        status = Pmic_wdgEnable(coreHandle);
    }

    if(status == PMIC_ST_SUCCESS)
    {
        /* Configure watchdog for Q&A mode */
        status = Pmic_wdgSetCfg(coreHandle, &gPmicWdgCfg[CONFIG_PMIC0]);
        /* Clear all interrupts */
        status += Pmic_irqClrErrStatus(coreHandle, PMIC_IRQ_ALL);
    }

    if(status == PMIC_ST_SUCCESS)
    {
        DebugP_log("Configure PMIC watchdog in Q&A mode... DONE \r\n");
        /* Begin watchdog Q&A sequence */
        status = Pmic_wdgBeginSequences(coreHandle, pmicConfig.wdogMode);
        if(status == PMIC_ST_SUCCESS)
        {
            /* Exit Long Window */
            for (answerCnt = 4; answerCnt != 0; answerCnt--)
            {
                status = Pmic_wdgQaSequenceWriteAnswer(coreHandle);
                if(status != PMIC_ST_SUCCESS)
                {
                    break;
                }

                /* This check is not required in the actual application. This is to detect
                 * watchdog errors if any, when changing the sysconfig watchdog configuration
                 * during development.
                 */
                PMICApp_checkForWdgErrors(coreHandle);
            }

            if(status == PMIC_ST_SUCCESS)
            {
                DebugP_log("Started watchdog sequence... Sending answers to the watchdog... \r\n");
                /* Send watchdog answers for some iterations */
                for (numSeqeunces = 40; numSeqeunces != 0; numSeqeunces--)
                {
                    /* Enter Window-1 to write Answer-3, Answer-2, and Answer-1 */
                    for (answerCnt = 3; answerCnt >= 1; answerCnt--)
                    {
                        status = Pmic_wdgQaSequenceWriteAnswer(coreHandle);
                        if(status != PMIC_ST_SUCCESS)
                        {
                            break;
                        }

                        /* This check is not required in the actual application. This is to detect
                         * watchdog errors, if any when changing the sysconfig watchdog configuration
                         * during development.
                         */
                        PMICApp_checkForWdgErrors(coreHandle);
                    }

                    if(status == PMIC_ST_SUCCESS)
                    {
                        /* Wait until Window-1 time elapses */
                        ClockP_usleep(gPmicWdgCfg[CONFIG_PMIC0].win1Duration_us + ClockP_ticksToUsec(1));
                        /* Enter Window-2; calculate and send last answer byte */
                        status = Pmic_wdgQaSequenceWriteAnswer(coreHandle);

                        /* This check is not required in the actual application. This is to detect
                         * watchdog errors, if any when changing the sysconfig watchdog configuration
                         * during development.
                         */
                        PMICApp_checkForWdgErrors(coreHandle);
                    }

                    if(status != PMIC_ST_SUCCESS)
                    {
                        break;
                    }
                }
            }
        }
    }

    if(status == PMIC_ST_SUCCESS)
    {
        DebugP_log("Stopped sending watchdog answers... Waiting for the PMIC interrupt... \r\n");
        while(!isWdogInt)
        {
            /* Stop sending the Q&A signal and wait for PMIC GPIO interrupt */
            (void)SemaphoreP_pend(&gGpioISRDoneSem, SystemP_WAIT_FOREVER);
            /* Check whether the PMIC interrupt is due to watchdog error */
            status = Pmic_wdgGetErrorStatus(coreHandle, &wdgErrStat);
            if(status == PMIC_ST_SUCCESS)
            {
                if(wdgErrStat.wdFailInt == true)
                {
                    isWdogInt = true;
                }
                /* Clear all interrupts */
                status = Pmic_irqClrErrStatus(coreHandle, PMIC_IRQ_ALL);
            }
        }
    }

    if(status == PMIC_ST_SUCCESS)
    {
        DebugP_log("Received GPIO interrupt for PMIC watchdog error !! \r\n");
        /* Set return to long window */
        tmpWdgCfg.validParams = PMIC_CFG_WDG_RETLONGWIN_VALID_SHIFT;
        tmpWdgCfg.retLongWin = true;
        status = Pmic_wdgSetCfg(coreHandle, &tmpWdgCfg);
        if(status == PMIC_ST_SUCCESS)
        {
            /* Wait for current watchdog sequence to end and enter long window */
            ClockP_usleep(gPmicWdgCfg[CONFIG_PMIC0].win1Duration_us + \
                          gPmicWdgCfg[CONFIG_PMIC0].win2Duration_us + \
                          ClockP_ticksToUsec(1));
            /* Set the PWRHOLD, so watchdog remains in long window */
            tmpWdgCfg.validParams = PMIC_CFG_WDG_PWRHOLD_VALID_SHIFT;
            tmpWdgCfg.pwrHold = true;
            status = Pmic_wdgSetCfg(coreHandle, &tmpWdgCfg);
        }
    }

    if(status == PMIC_ST_SUCCESS)
    {
        DebugP_log("PMIC Q&A watchdog interrupt mode test... DONE\r\n");
        retVal = SystemP_SUCCESS;
    }
    else
    {
        DebugP_logError("PMIC Q&A watchdog interrupt mode test... Failed !!\r\n");
    }

    return retVal;
}

static void PMICApp_checkForWdgErrors(Pmic_CoreHandle_t *coreHandle)
{
    int32_t       status = PMIC_ST_SUCCESS;

    Pmic_WdgErrStatus_t wdgErrStat =
    {
        .validParams = (PMIC_CFG_WD_FAIL_INT_ERRSTAT_VALID_SHIFT | \
                        PMIC_CFG_WD_LONGWIN_TIMEOUT_ERRSTAT_VALID_SHIFT | \
                        PMIC_CFG_WD_RST_INT_ERRSTAT_VALID_SHIFT | \
                        PMIC_CFG_WD_ANSW_EARLY_ERRSTAT_VALID_SHIFT | \
                        PMIC_CFG_WD_SEQ_ERR_ERRSTAT_VALID_SHIFT | \
                        PMIC_CFG_WD_ANSW_ERR_ERRSTAT_VALID_SHIFT | \
                        PMIC_CFG_WD_TIMEOUT_ERRSTAT_VALID_SHIFT),
    };

    status = Pmic_wdgGetErrorStatus(coreHandle, &wdgErrStat);
    DebugP_assert(status == PMIC_ST_SUCCESS && \
                  !(wdgErrStat.wdLongWinTimeout || \
                  wdgErrStat.wdTimeout || \
                  wdgErrStat.wdFailInt || \
                  wdgErrStat.wdAnswearly || \
                  wdgErrStat.wdSeqErr || \
                  wdgErrStat.wdAnswErr || \
                  wdgErrStat.wdRstInt));

    return;
}

static void PMICApp_gpioISR(void *args)
{
    uint32_t    pinNum = (uint32_t) args;
    uint32_t    bankNum =  GPIO_GET_BANK_INDEX(pinNum);
    uint32_t    intrStatus, pinMask = GPIO_GET_BANK_BIT_MASK(pinNum);

    /* Get and clear bank interrupt status */
    intrStatus = GPIO_getBankIntrStatus(gGpioWdogIntBaseAddr, bankNum);
    GPIO_clearBankIntrStatus(gGpioWdogIntBaseAddr, bankNum, intrStatus);

    /* Post the semaphore if it is watchdog GPIO interrupt */
    if(intrStatus & pinMask)
    {
        SemaphoreP_post(&gGpioISRDoneSem);
    }

    return;
}