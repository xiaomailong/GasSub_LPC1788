/**********************************************************************
* $Id$		sdram_mt48lc8m32lfb5.c			2011-06-02
*//**
* @file		sdram_mt48lc8m32lfb5.c
* @brief	Contains all functions support for Micron MT48LC8M32LFB5
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "bsp.h"
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc177x_8x_libcfg.h"
#else
#include "lpc177x_8x_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */

#ifdef _EMC
#include "bsp.h"
#include "lpc177x_8x_emc.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "sdram_mt48lc8m32lfb5.h"

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup Sdram_MT48LC8M32FLB5
 * @{
 */

/*********************************************************************//**
 * @brief 		Initialize external SDRAM memory Micron MT48LC8M32LFB5
 *				256Mbit(8M x 32)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SDRAMInit( void )
{
    volatile uint32_t i;
    volatile uint32_t dwtemp;
    EMC_DYN_MEM_Config_Type config;
    TIM_TIMERCFG_Type TIM_ConfigStruct;

    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
    TIM_ConfigStruct.PrescaleValue  = 1;
      
    // Set configuration for Tim_config and Tim_MatchConfig
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);
      
    config.ChipSize = 128;
    config.AddrBusWidth = 32;
    config.AddrMap = EMC_ADD_MAP_ROW_BANK_COL;
    config.CSn = 0;
    config.DataWidth = 32;
    config.TotalSize = SDRAM_SIZE;

    config.CASLatency = 3;
    config.RASLatency = 3;
    config.Active2ActivePeriod = P2C(SDRAM_TRC);
    config.ActiveBankLatency = P2C(SDRAM_TRRD);
    config.AutoRefrehPeriod = P2C(SDRAM_TRFC);
    config.DataIn2ActiveTime = SDRAM_TDAL + P2C(SDRAM_TRP);
    config.DataOut2ActiveTime = SDRAM_TAPR;
    config.WriteRecoveryTime = SDRAM_TWR;
    config.ExitSelfRefreshTime = P2C(SDRAM_TXSR);
    config.LoadModeReg2Active = SDRAM_TMRD;
    config.PrechargeCmdPeriod = P2C(SDRAM_TRP);
    config.ReadConfig = 1;  /* Command delayed strategy, using EMCCLKDELAY */
    config.RefreshTime = 0x1f;
    config.Active2PreChargeTime = P2C(SDRAM_TRAS);
    config.SeftRefreshExitTime = P2C(SDRAM_TXSR);
    DynMem_Init(&config);

    // TIM_Waitms(100);
    EMC_DynCtrlSDRAMInit(EMC_DYNAMIC_CTRL_SDRAM_NOP); /* Issue NOP command */

    // TIM_Waitms(100);                  /* wait 200ms */
    EMC_DynCtrlSDRAMInit(EMC_DYNAMIC_CTRL_SDRAM_PALL); /* Issue Pre-charge command */

    for(i = 0; i < 0x80; i++);         /* wait 128 AHB clock cycles */
    
    // TIM_Waitms(100);    
    EMC_DynCtrlSDRAMInit(EMC_DYNAMIC_CTRL_SDRAM_MODE); /* Issue MODE command */
    dwtemp = *((volatile uint32_t *)(SDRAM_BASE_ADDR | (0x32<<(2+2+8)))); /* Mode Register Setting: 4 burst, 2 CAS latency */

    //Timing for 48/60/72MHZ Bus
    EMC_DynCtrlSDRAMInit(EMC_DYNAMIC_CTRL_SDRAM_NORMAL); /* Issue NORMAL command */

    //enable buffers
    EMC_DynMemConfigB(0, EMC_DYNAMIC_CFG_BUFF_ENABLED);

    TIM_DeInit(LPC_TIM0);
}
#endif /*_EMC*/

/**
 * @}
 */

/*********************************************************************************
**                            End Of File
*********************************************************************************/
