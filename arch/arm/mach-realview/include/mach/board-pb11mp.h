/*
 * arch/arm/mach-realview/include/mach/board-pb11mp.h
 *
 * Copyright (C) 2008 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef __ASM_ARCH_BOARD_PB11MP_H
#define __ASM_ARCH_BOARD_PB11MP_H

#include <mach/platform.h>

/*
                       
 */
#define REALVIEW_PB11MP_UART0_BASE		0x10009000	/*        */
#define REALVIEW_PB11MP_UART1_BASE		0x1000A000	/*        */
#define REALVIEW_PB11MP_UART2_BASE		0x1000B000	/*        */
#define REALVIEW_PB11MP_UART3_BASE		0x1000C000	/*        */
#define REALVIEW_PB11MP_SSP_BASE		0x1000D000	/*                         */
#define REALVIEW_PB11MP_WATCHDOG0_BASE		0x1000F000	/*            */
#define REALVIEW_PB11MP_WATCHDOG_BASE		0x10010000	/*                    */
#define REALVIEW_PB11MP_TIMER0_1_BASE		0x10011000	/*               */
#define REALVIEW_PB11MP_TIMER2_3_BASE		0x10012000	/*               */
#define REALVIEW_PB11MP_GPIO0_BASE		0x10013000	/*             */
#define REALVIEW_PB11MP_RTC_BASE		0x10017000	/*                 */
#define REALVIEW_PB11MP_TIMER4_5_BASE		0x10018000	/*           */
#define REALVIEW_PB11MP_TIMER6_7_BASE		0x10019000	/*           */
#define REALVIEW_PB11MP_SCTL_BASE		0x1001A000	/*                   */
#define REALVIEW_PB11MP_CLCD_BASE		0x10020000	/*      */
#define REALVIEW_PB11MP_ONB_SRAM_BASE		0x10060000	/*               */
#define REALVIEW_PB11MP_DMC_BASE		0x100E0000	/*                   */
#define REALVIEW_PB11MP_SMC_BASE		0x100E1000	/*                   */
#define REALVIEW_PB11MP_CAN_BASE		0x100E2000	/*         */
#define REALVIEW_PB11MP_CF_BASE			0x18000000	/*               */
#define REALVIEW_PB11MP_CF_MEM_BASE		0x18003000	/*                       */
#define REALVIEW_PB11MP_GIC_CPU_BASE		0x1E000000	/*                                            */
#define REALVIEW_PB11MP_FLASH0_BASE		0x40000000
#define REALVIEW_PB11MP_FLASH0_SIZE		SZ_64M
#define REALVIEW_PB11MP_FLASH1_BASE		0x44000000
#define REALVIEW_PB11MP_FLASH1_SIZE		SZ_64M
#define REALVIEW_PB11MP_ETH_BASE		0x4E000000	/*          */
#define REALVIEW_PB11MP_USB_BASE		0x4F000000	/*     */
#define REALVIEW_PB11MP_GIC_DIST_BASE		0x1E001000	/*                                          */
#define REALVIEW_PB11MP_LT_BASE			0xC0000000	/*                      */
#define REALVIEW_PB11MP_SDRAM6_BASE		0x70000000	/*                    */
#define REALVIEW_PB11MP_SDRAM7_BASE		0x80000000	/*                    */

#define REALVIEW_PB11MP_SYS_PLD_CTRL1		0x74

/*
                         
 */
#define REALVIEW_PB11MP_PCI_BASE		0x90040000	/*                 */
#define REALVIEW_PB11MP_PCI_IO_BASE		0x90050000	/*                  */
#define REALVIEW_PB11MP_PCI_MEM_BASE		0xA0000000	/*                   */

#define REALVIEW_PB11MP_PCI_BASE_SIZE		0x10000		/*       */
#define REALVIEW_PB11MP_PCI_IO_SIZE		0x1000		/*      */
#define REALVIEW_PB11MP_PCI_MEM_SIZE		0x20000000	/*        */

/*
                                           
 */
#define REALVIEW_TC11MP_PRIV_MEM_BASE		0x1F000000
#define REALVIEW_TC11MP_PRIV_MEM_SIZE		SZ_8K
#define REALVIEW_TC11MP_SCU_BASE		0x1F000000	/*                */
#define REALVIEW_TC11MP_GIC_CPU_BASE		0x1F000100	/*                                              */
#define REALVIEW_TC11MP_TWD_BASE		0x1F000600
#define REALVIEW_TC11MP_GIC_DIST_BASE		0x1F001000	/*                                            */
#define REALVIEW_TC11MP_L220_BASE		0x1F002000	/*                */

 /*
                                     
 */
#define REALVIEW_PB11MP_SYS_CTRL_RESET_CONFIGCLR    0x01
#define REALVIEW_PB11MP_SYS_CTRL_RESET_CONFIGINIT   0x02
#define REALVIEW_PB11MP_SYS_CTRL_RESET_DLLRESET     0x03
#define REALVIEW_PB11MP_SYS_CTRL_RESET_PLLRESET     0x04
#define REALVIEW_PB11MP_SYS_CTRL_RESET_POR          0x05
#define REALVIEW_PB11MP_SYS_CTRL_RESET_DoC          0x06

#define REALVIEW_PB11MP_SYS_CTRL_LED         (1 << 0)

#endif	/*                           */
