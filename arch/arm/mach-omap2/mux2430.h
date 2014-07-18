/*
 * Copyright (C) 2009 Nokia
 * Copyright (C) 2009 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define OMAP2430_CONTROL_PADCONF_MUX_PBASE			0x49002030LU

#define OMAP2430_MUX(mode0, mux_value)					\
{									\
	.reg_offset	= (OMAP2430_CONTROL_PADCONF_##mode0##_OFFSET),	\
	.value		= (mux_value),					\
}

/*
                                                            
  
                                                                     
                                                                   
                                                   
  
                                                                     
                                  
 */
#define OMAP2430_CONTROL_PADCONF_GPMC_CLK_OFFSET		0x000
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS0_OFFSET		0x001
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS1_OFFSET		0x002
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS2_OFFSET		0x003
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS3_OFFSET		0x004
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS4_OFFSET		0x005
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS5_OFFSET		0x006
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS6_OFFSET		0x007
#define OMAP2430_CONTROL_PADCONF_GPMC_NCS7_OFFSET		0x008
#define OMAP2430_CONTROL_PADCONF_GPMC_NADV_ALE_OFFSET		0x009
#define OMAP2430_CONTROL_PADCONF_GPMC_NOE_NRE_OFFSET		0x00a
#define OMAP2430_CONTROL_PADCONF_GPMC_NWE_OFFSET		0x00b
#define OMAP2430_CONTROL_PADCONF_GPMC_NBE0_CLE_OFFSET		0x00c
#define OMAP2430_CONTROL_PADCONF_GPMC_NBE1_OFFSET		0x00d
#define OMAP2430_CONTROL_PADCONF_GPMC_NWP_OFFSET		0x00e
#define OMAP2430_CONTROL_PADCONF_GPMC_WAIT0_OFFSET		0x00f
#define OMAP2430_CONTROL_PADCONF_GPMC_WAIT1_OFFSET		0x010
#define OMAP2430_CONTROL_PADCONF_GPMC_WAIT2_OFFSET		0x011
#define OMAP2430_CONTROL_PADCONF_GPMC_WAIT3_OFFSET		0x012
#define OMAP2430_CONTROL_PADCONF_SDRC_CLK_OFFSET		0x013
#define OMAP2430_CONTROL_PADCONF_SDRC_NCLK_OFFSET		0x014
#define OMAP2430_CONTROL_PADCONF_SDRC_NCS0_OFFSET		0x015
#define OMAP2430_CONTROL_PADCONF_SDRC_NCS1_OFFSET		0x016
#define OMAP2430_CONTROL_PADCONF_SDRC_CKE0_OFFSET		0x017
#define OMAP2430_CONTROL_PADCONF_SDRC_CKE1_OFFSET		0x018
#define OMAP2430_CONTROL_PADCONF_SDRC_NRAS_OFFSET		0x019
#define OMAP2430_CONTROL_PADCONF_SDRC_NCAS_OFFSET		0x01a
#define OMAP2430_CONTROL_PADCONF_SDRC_NWE_OFFSET		0x01b
#define OMAP2430_CONTROL_PADCONF_SDRC_DM0_OFFSET		0x01c
#define OMAP2430_CONTROL_PADCONF_SDRC_DM1_OFFSET		0x01d
#define OMAP2430_CONTROL_PADCONF_SDRC_DM2_OFFSET		0x01e
#define OMAP2430_CONTROL_PADCONF_SDRC_DM3_OFFSET		0x01f
#define OMAP2430_CONTROL_PADCONF_SDRC_DQS0_OFFSET		0x020
#define OMAP2430_CONTROL_PADCONF_SDRC_DQS1_OFFSET		0x021
#define OMAP2430_CONTROL_PADCONF_SDRC_DQS2_OFFSET		0x022
#define OMAP2430_CONTROL_PADCONF_SDRC_DQS3_OFFSET		0x023
#define OMAP2430_CONTROL_PADCONF_SDRC_A14_OFFSET		0x024
#define OMAP2430_CONTROL_PADCONF_SDRC_A13_OFFSET		0x025
#define OMAP2430_CONTROL_PADCONF_SDRC_A12_OFFSET		0x026
#define OMAP2430_CONTROL_PADCONF_SDRC_BA1_OFFSET		0x027
#define OMAP2430_CONTROL_PADCONF_SDRC_BA0_OFFSET		0x028
#define OMAP2430_CONTROL_PADCONF_SDRC_A11_OFFSET		0x029
#define OMAP2430_CONTROL_PADCONF_SDRC_A10_OFFSET		0x02a
#define OMAP2430_CONTROL_PADCONF_SDRC_A9_OFFSET			0x02b
#define OMAP2430_CONTROL_PADCONF_SDRC_A8_OFFSET			0x02c
#define OMAP2430_CONTROL_PADCONF_SDRC_A7_OFFSET			0x02d
#define OMAP2430_CONTROL_PADCONF_SDRC_A6_OFFSET			0x02e
#define OMAP2430_CONTROL_PADCONF_SDRC_A5_OFFSET			0x02f
#define OMAP2430_CONTROL_PADCONF_SDRC_A4_OFFSET			0x030
#define OMAP2430_CONTROL_PADCONF_SDRC_A3_OFFSET			0x031
#define OMAP2430_CONTROL_PADCONF_SDRC_A2_OFFSET			0x032
#define OMAP2430_CONTROL_PADCONF_SDRC_A1_OFFSET			0x033
#define OMAP2430_CONTROL_PADCONF_SDRC_A0_OFFSET			0x034
#define OMAP2430_CONTROL_PADCONF_SDRC_D31_OFFSET		0x035
#define OMAP2430_CONTROL_PADCONF_SDRC_D30_OFFSET		0x036
#define OMAP2430_CONTROL_PADCONF_SDRC_D29_OFFSET		0x037
#define OMAP2430_CONTROL_PADCONF_SDRC_D28_OFFSET		0x038
#define OMAP2430_CONTROL_PADCONF_SDRC_D27_OFFSET		0x039
#define OMAP2430_CONTROL_PADCONF_SDRC_D26_OFFSET		0x03a
#define OMAP2430_CONTROL_PADCONF_SDRC_D25_OFFSET		0x03b
#define OMAP2430_CONTROL_PADCONF_SDRC_D24_OFFSET		0x03c
#define OMAP2430_CONTROL_PADCONF_SDRC_D23_OFFSET		0x03d
#define OMAP2430_CONTROL_PADCONF_SDRC_D22_OFFSET		0x03e
#define OMAP2430_CONTROL_PADCONF_SDRC_D21_OFFSET		0x03f
#define OMAP2430_CONTROL_PADCONF_SDRC_D20_OFFSET		0x040
#define OMAP2430_CONTROL_PADCONF_SDRC_D19_OFFSET		0x041
#define OMAP2430_CONTROL_PADCONF_SDRC_D18_OFFSET		0x042
#define OMAP2430_CONTROL_PADCONF_SDRC_D17_OFFSET		0x043
#define OMAP2430_CONTROL_PADCONF_SDRC_D16_OFFSET		0x044
#define OMAP2430_CONTROL_PADCONF_SDRC_D15_OFFSET		0x045
#define OMAP2430_CONTROL_PADCONF_SDRC_D14_OFFSET		0x046
#define OMAP2430_CONTROL_PADCONF_SDRC_D13_OFFSET		0x047
#define OMAP2430_CONTROL_PADCONF_SDRC_D12_OFFSET		0x048
#define OMAP2430_CONTROL_PADCONF_SDRC_D11_OFFSET		0x049
#define OMAP2430_CONTROL_PADCONF_SDRC_D10_OFFSET		0x04a
#define OMAP2430_CONTROL_PADCONF_SDRC_D9_OFFSET			0x04b
#define OMAP2430_CONTROL_PADCONF_SDRC_D8_OFFSET			0x04c
#define OMAP2430_CONTROL_PADCONF_SDRC_D7_OFFSET			0x04d
#define OMAP2430_CONTROL_PADCONF_SDRC_D6_OFFSET			0x04e
#define OMAP2430_CONTROL_PADCONF_SDRC_D5_OFFSET			0x04f
#define OMAP2430_CONTROL_PADCONF_SDRC_D4_OFFSET			0x050
#define OMAP2430_CONTROL_PADCONF_SDRC_D3_OFFSET			0x051
#define OMAP2430_CONTROL_PADCONF_SDRC_D2_OFFSET			0x052
#define OMAP2430_CONTROL_PADCONF_SDRC_D1_OFFSET			0x053
#define OMAP2430_CONTROL_PADCONF_SDRC_D0_OFFSET			0x054
#define OMAP2430_CONTROL_PADCONF_GPMC_A10_OFFSET		0x055
#define OMAP2430_CONTROL_PADCONF_GPMC_A9_OFFSET			0x056
#define OMAP2430_CONTROL_PADCONF_GPMC_A8_OFFSET			0x057
#define OMAP2430_CONTROL_PADCONF_GPMC_A7_OFFSET			0x058
#define OMAP2430_CONTROL_PADCONF_GPMC_A6_OFFSET			0x059
#define OMAP2430_CONTROL_PADCONF_GPMC_A5_OFFSET			0x05a
#define OMAP2430_CONTROL_PADCONF_GPMC_A4_OFFSET			0x05b
#define OMAP2430_CONTROL_PADCONF_GPMC_A3_OFFSET			0x05c
#define OMAP2430_CONTROL_PADCONF_GPMC_A2_OFFSET			0x05d
#define OMAP2430_CONTROL_PADCONF_GPMC_A1_OFFSET			0x05e
#define OMAP2430_CONTROL_PADCONF_GPMC_D15_OFFSET		0x05f
#define OMAP2430_CONTROL_PADCONF_GPMC_D14_OFFSET		0x060
#define OMAP2430_CONTROL_PADCONF_GPMC_D13_OFFSET		0x061
#define OMAP2430_CONTROL_PADCONF_GPMC_D12_OFFSET		0x062
#define OMAP2430_CONTROL_PADCONF_GPMC_D11_OFFSET		0x063
#define OMAP2430_CONTROL_PADCONF_GPMC_D10_OFFSET		0x064
#define OMAP2430_CONTROL_PADCONF_GPMC_D9_OFFSET			0x065
#define OMAP2430_CONTROL_PADCONF_GPMC_D8_OFFSET			0x066
#define OMAP2430_CONTROL_PADCONF_GPMC_D7_OFFSET			0x067
#define OMAP2430_CONTROL_PADCONF_GPMC_D6_OFFSET			0x068
#define OMAP2430_CONTROL_PADCONF_GPMC_D5_OFFSET			0x069
#define OMAP2430_CONTROL_PADCONF_GPMC_D4_OFFSET			0x06a
#define OMAP2430_CONTROL_PADCONF_GPMC_D3_OFFSET			0x06b
#define OMAP2430_CONTROL_PADCONF_GPMC_D2_OFFSET			0x06c
#define OMAP2430_CONTROL_PADCONF_GPMC_D1_OFFSET			0x06d
#define OMAP2430_CONTROL_PADCONF_GPMC_D0_OFFSET			0x06e
#define OMAP2430_CONTROL_PADCONF_DSS_DATA0_OFFSET		0x06f
#define OMAP2430_CONTROL_PADCONF_DSS_DATA1_OFFSET		0x070
#define OMAP2430_CONTROL_PADCONF_DSS_DATA2_OFFSET		0x071
#define OMAP2430_CONTROL_PADCONF_DSS_DATA3_OFFSET		0x072
#define OMAP2430_CONTROL_PADCONF_DSS_DATA4_OFFSET		0x073
#define OMAP2430_CONTROL_PADCONF_DSS_DATA5_OFFSET		0x074
#define OMAP2430_CONTROL_PADCONF_DSS_DATA6_OFFSET		0x075
#define OMAP2430_CONTROL_PADCONF_DSS_DATA7_OFFSET		0x076
#define OMAP2430_CONTROL_PADCONF_DSS_DATA8_OFFSET		0x077
#define OMAP2430_CONTROL_PADCONF_DSS_DATA9_OFFSET		0x078
#define OMAP2430_CONTROL_PADCONF_DSS_DATA10_OFFSET		0x079
#define OMAP2430_CONTROL_PADCONF_DSS_DATA11_OFFSET		0x07a
#define OMAP2430_CONTROL_PADCONF_DSS_DATA12_OFFSET		0x07b
#define OMAP2430_CONTROL_PADCONF_DSS_DATA13_OFFSET		0x07c
#define OMAP2430_CONTROL_PADCONF_DSS_DATA14_OFFSET		0x07d
#define OMAP2430_CONTROL_PADCONF_DSS_DATA15_OFFSET		0x07e
#define OMAP2430_CONTROL_PADCONF_DSS_DATA16_OFFSET		0x07f
#define OMAP2430_CONTROL_PADCONF_DSS_DATA17_OFFSET		0x080
#define OMAP2430_CONTROL_PADCONF_UART1_CTS_OFFSET		0x081
#define OMAP2430_CONTROL_PADCONF_UART1_RTS_OFFSET		0x082
#define OMAP2430_CONTROL_PADCONF_UART1_TX_OFFSET		0x083
#define OMAP2430_CONTROL_PADCONF_UART1_RX_OFFSET		0x084
#define OMAP2430_CONTROL_PADCONF_MCBSP2_DR_OFFSET		0x085
#define OMAP2430_CONTROL_PADCONF_MCBSP2_CLKX_OFFSET		0x086
#define OMAP2430_CONTROL_PADCONF_DSS_PCLK_OFFSET		0x087
#define OMAP2430_CONTROL_PADCONF_DSS_VSYNC_OFFSET		0x088
#define OMAP2430_CONTROL_PADCONF_DSS_HSYNC_OFFSET		0x089
#define OMAP2430_CONTROL_PADCONF_DSS_ACBIAS_OFFSET		0x08a
#define OMAP2430_CONTROL_PADCONF_SYS_NRESPWRON_OFFSET		0x08b
#define OMAP2430_CONTROL_PADCONF_SYS_NRESWARM_OFFSET		0x08c
#define OMAP2430_CONTROL_PADCONF_SYS_NIRQ0_OFFSET		0x08d
#define OMAP2430_CONTROL_PADCONF_SYS_NIRQ1_OFFSET		0x08e
#define OMAP2430_CONTROL_PADCONF_SYS_VMODE_OFFSET		0x08f
#define OMAP2430_CONTROL_PADCONF_GPIO_128_OFFSET		0x090
#define OMAP2430_CONTROL_PADCONF_GPIO_129_OFFSET		0x091
#define OMAP2430_CONTROL_PADCONF_GPIO_130_OFFSET		0x092
#define OMAP2430_CONTROL_PADCONF_GPIO_131_OFFSET		0x093
#define OMAP2430_CONTROL_PADCONF_SYS_32K_OFFSET			0x094
#define OMAP2430_CONTROL_PADCONF_SYS_XTALIN_OFFSET		0x095
#define OMAP2430_CONTROL_PADCONF_SYS_XTALOUT_OFFSET		0x096
#define OMAP2430_CONTROL_PADCONF_GPIO_132_OFFSET		0x097
#define OMAP2430_CONTROL_PADCONF_SYS_CLKREQ_OFFSET		0x098
#define OMAP2430_CONTROL_PADCONF_SYS_CLKOUT_OFFSET		0x099
#define OMAP2430_CONTROL_PADCONF_GPIO_151_OFFSET		0x09a
#define OMAP2430_CONTROL_PADCONF_GPIO_133_OFFSET		0x09b
#define OMAP2430_CONTROL_PADCONF_JTAG_EMU1_OFFSET		0x09c
#define OMAP2430_CONTROL_PADCONF_JTAG_EMU0_OFFSET		0x09d
#define OMAP2430_CONTROL_PADCONF_JTAG_NTRST_OFFSET		0x09e
#define OMAP2430_CONTROL_PADCONF_JTAG_TCK_OFFSET		0x09f
#define OMAP2430_CONTROL_PADCONF_JTAG_RTCK_OFFSET		0x0a0
#define OMAP2430_CONTROL_PADCONF_JTAG_TMS_OFFSET		0x0a1
#define OMAP2430_CONTROL_PADCONF_JTAG_TDI_OFFSET		0x0a2
#define OMAP2430_CONTROL_PADCONF_JTAG_TDO_OFFSET		0x0a3
#define OMAP2430_CONTROL_PADCONF_CAM_D9_OFFSET			0x0a4
#define OMAP2430_CONTROL_PADCONF_CAM_D8_OFFSET			0x0a5
#define OMAP2430_CONTROL_PADCONF_CAM_D7_OFFSET			0x0a6
#define OMAP2430_CONTROL_PADCONF_CAM_D6_OFFSET			0x0a7
#define OMAP2430_CONTROL_PADCONF_CAM_D5_OFFSET			0x0a8
#define OMAP2430_CONTROL_PADCONF_CAM_D4_OFFSET			0x0a9
#define OMAP2430_CONTROL_PADCONF_CAM_D3_OFFSET			0x0aa
#define OMAP2430_CONTROL_PADCONF_CAM_D2_OFFSET			0x0ab
#define OMAP2430_CONTROL_PADCONF_CAM_D1_OFFSET			0x0ac
#define OMAP2430_CONTROL_PADCONF_CAM_D0_OFFSET			0x0ad
#define OMAP2430_CONTROL_PADCONF_CAM_HS_OFFSET			0x0ae
#define OMAP2430_CONTROL_PADCONF_CAM_VS_OFFSET			0x0af
#define OMAP2430_CONTROL_PADCONF_CAM_LCLK_OFFSET		0x0b0
#define OMAP2430_CONTROL_PADCONF_CAM_XCLK_OFFSET		0x0b1
#define OMAP2430_CONTROL_PADCONF_CAM_D11_OFFSET			0x0b2
#define OMAP2430_CONTROL_PADCONF_CAM_D10_OFFSET			0x0b3
#define OMAP2430_CONTROL_PADCONF_GPIO_134_OFFSET		0x0b4
#define OMAP2430_CONTROL_PADCONF_GPIO_135_OFFSET		0x0b5
#define OMAP2430_CONTROL_PADCONF_GPIO_136_OFFSET		0x0b6
#define OMAP2430_CONTROL_PADCONF_GPIO_137_OFFSET		0x0b7
#define OMAP2430_CONTROL_PADCONF_GPIO_138_OFFSET		0x0b8
#define OMAP2430_CONTROL_PADCONF_GPIO_139_OFFSET		0x0b9
#define OMAP2430_CONTROL_PADCONF_GPIO_140_OFFSET		0x0ba
#define OMAP2430_CONTROL_PADCONF_GPIO_141_OFFSET		0x0bb
#define OMAP2430_CONTROL_PADCONF_GPIO_142_OFFSET		0x0bc
#define OMAP2430_CONTROL_PADCONF_GPIO_154_OFFSET		0x0bd
#define OMAP2430_CONTROL_PADCONF_GPIO_148_OFFSET		0x0be
#define OMAP2430_CONTROL_PADCONF_GPIO_149_OFFSET		0x0bf
#define OMAP2430_CONTROL_PADCONF_GPIO_150_OFFSET		0x0c0
#define OMAP2430_CONTROL_PADCONF_GPIO_152_OFFSET		0x0c1
#define OMAP2430_CONTROL_PADCONF_GPIO_153_OFFSET		0x0c2
#define OMAP2430_CONTROL_PADCONF_SDMMC1_CLKO_OFFSET		0x0c3
#define OMAP2430_CONTROL_PADCONF_SDMMC1_CMD_OFFSET		0x0c4
#define OMAP2430_CONTROL_PADCONF_SDMMC1_DAT0_OFFSET		0x0c5
#define OMAP2430_CONTROL_PADCONF_SDMMC1_DAT1_OFFSET		0x0c6
#define OMAP2430_CONTROL_PADCONF_SDMMC1_DAT2_OFFSET		0x0c7
#define OMAP2430_CONTROL_PADCONF_SDMMC1_DAT3_OFFSET		0x0c8
#define OMAP2430_CONTROL_PADCONF_SDMMC2_CLKO_OFFSET		0x0c9
#define OMAP2430_CONTROL_PADCONF_SDMMC2_DAT3_OFFSET		0x0ca
#define OMAP2430_CONTROL_PADCONF_SDMMC2_CMD_OFFSET		0x0cb
#define OMAP2430_CONTROL_PADCONF_SDMMC2_DAT0_OFFSET		0x0cc
#define OMAP2430_CONTROL_PADCONF_SDMMC2_DAT2_OFFSET		0x0cd
#define OMAP2430_CONTROL_PADCONF_SDMMC2_DAT1_OFFSET		0x0ce
#define OMAP2430_CONTROL_PADCONF_UART2_CTS_OFFSET		0x0cf
#define OMAP2430_CONTROL_PADCONF_UART2_RTS_OFFSET		0x0d0
#define OMAP2430_CONTROL_PADCONF_UART2_TX_OFFSET		0x0d1
#define OMAP2430_CONTROL_PADCONF_UART2_RX_OFFSET		0x0d2
#define OMAP2430_CONTROL_PADCONF_MCBSP3_CLKX_OFFSET		0x0d3
#define OMAP2430_CONTROL_PADCONF_MCBSP3_FSX_OFFSET		0x0d4
#define OMAP2430_CONTROL_PADCONF_MCBSP3_DR_OFFSET		0x0d5
#define OMAP2430_CONTROL_PADCONF_MCBSP3_DX_OFFSET		0x0d6
#define OMAP2430_CONTROL_PADCONF_SSI1_DAT_TX_OFFSET		0x0d7
#define OMAP2430_CONTROL_PADCONF_SSI1_FLAG_TX_OFFSET		0x0d8
#define OMAP2430_CONTROL_PADCONF_SSI1_RDY_TX_OFFSET		0x0d9
#define OMAP2430_CONTROL_PADCONF_SSI1_DAT_RX_OFFSET		0x0da
#define OMAP2430_CONTROL_PADCONF_GPIO_63_OFFSET			0x0db
#define OMAP2430_CONTROL_PADCONF_SSI1_FLAG_RX_OFFSET		0x0dc
#define OMAP2430_CONTROL_PADCONF_SSI1_RDY_RX_OFFSET		0x0dd
#define OMAP2430_CONTROL_PADCONF_SSI1_WAKE_OFFSET		0x0de
#define OMAP2430_CONTROL_PADCONF_SPI1_CLK_OFFSET		0x0df
#define OMAP2430_CONTROL_PADCONF_SPI1_SIMO_OFFSET		0x0e0
#define OMAP2430_CONTROL_PADCONF_SPI1_SOMI_OFFSET		0x0e1
#define OMAP2430_CONTROL_PADCONF_SPI1_CS0_OFFSET		0x0e2
#define OMAP2430_CONTROL_PADCONF_SPI1_CS1_OFFSET		0x0e3
#define OMAP2430_CONTROL_PADCONF_SPI1_CS2_OFFSET		0x0e4
#define OMAP2430_CONTROL_PADCONF_SPI1_CS3_OFFSET		0x0e5
#define OMAP2430_CONTROL_PADCONF_SPI2_CLK_OFFSET		0x0e6
#define OMAP2430_CONTROL_PADCONF_SPI2_SIMO_OFFSET		0x0e7
#define OMAP2430_CONTROL_PADCONF_SPI2_SOMI_OFFSET		0x0e8
#define OMAP2430_CONTROL_PADCONF_SPI2_CS0_OFFSET		0x0e9
#define OMAP2430_CONTROL_PADCONF_MCBSP1_CLKR_OFFSET		0x0ea
#define OMAP2430_CONTROL_PADCONF_MCBSP1_FSR_OFFSET		0x0eb
#define OMAP2430_CONTROL_PADCONF_MCBSP1_DX_OFFSET		0x0ec
#define OMAP2430_CONTROL_PADCONF_MCBSP1_DR_OFFSET		0x0ed
#define OMAP2430_CONTROL_PADCONF_MCBSP_CLKS_OFFSET		0x0ee
#define OMAP2430_CONTROL_PADCONF_MCBSP1_FSX_OFFSET		0x0ef
#define OMAP2430_CONTROL_PADCONF_MCBSP1_CLKX_OFFSET		0x0f0
#define OMAP2430_CONTROL_PADCONF_I2C1_SCL_OFFSET		0x0f1
#define OMAP2430_CONTROL_PADCONF_I2C1_SDA_OFFSET		0x0f2
#define OMAP2430_CONTROL_PADCONF_I2C2_SCL_OFFSET		0x0f3
#define OMAP2430_CONTROL_PADCONF_I2C2_SDA_OFFSET		0x0f4
#define OMAP2430_CONTROL_PADCONF_HDQ_SIO_OFFSET			0x0f5
#define OMAP2430_CONTROL_PADCONF_UART3_CTS_RCTX_OFFSET		0x0f6
#define OMAP2430_CONTROL_PADCONF_UART3_RTS_SD_OFFSET		0x0f7
#define OMAP2430_CONTROL_PADCONF_UART3_TX_IRTX_OFFSET		0x0f8
#define OMAP2430_CONTROL_PADCONF_UART3_RX_IRRX_OFFSET		0x0f9
#define OMAP2430_CONTROL_PADCONF_GPIO_7_OFFSET			0x0fa
#define OMAP2430_CONTROL_PADCONF_GPIO_78_OFFSET			0x0fb
#define OMAP2430_CONTROL_PADCONF_GPIO_79_OFFSET			0x0fc
#define OMAP2430_CONTROL_PADCONF_GPIO_80_OFFSET			0x0fd
#define OMAP2430_CONTROL_PADCONF_GPIO_113_OFFSET		0x0fe
#define OMAP2430_CONTROL_PADCONF_GPIO_114_OFFSET		0x0ff
#define OMAP2430_CONTROL_PADCONF_GPIO_115_OFFSET		0x100
#define OMAP2430_CONTROL_PADCONF_GPIO_116_OFFSET		0x101
#define OMAP2430_CONTROL_PADCONF_SYS_DRM_MSECURE_OFFSET		0x102
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA3_OFFSET		0x103
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA4_OFFSET		0x104
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA5_OFFSET		0x105
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA6_OFFSET		0x106
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA2_OFFSET		0x107
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA0_OFFSET		0x108
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA1_OFFSET		0x109
#define OMAP2430_CONTROL_PADCONF_USB0HS_CLK_OFFSET		0x10a
#define OMAP2430_CONTROL_PADCONF_USB0HS_DIR_OFFSET		0x10b
#define OMAP2430_CONTROL_PADCONF_USB0HS_STP_OFFSET		0x10c
#define OMAP2430_CONTROL_PADCONF_USB0HS_NXT_OFFSET		0x10d
#define OMAP2430_CONTROL_PADCONF_USB0HS_DATA7_OFFSET		0x10e
#define OMAP2430_CONTROL_PADCONF_TV_OUT_OFFSET			0x10f
#define OMAP2430_CONTROL_PADCONF_TV_VREF_OFFSET			0x110
#define OMAP2430_CONTROL_PADCONF_TV_RSET_OFFSET			0x111
#define OMAP2430_CONTROL_PADCONF_TV_VFB_OFFSET			0x112
#define OMAP2430_CONTROL_PADCONF_TV_DACOUT_OFFSET		0x113
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD0_OFFSET		0x114
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD1_OFFSET		0x115
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD2_OFFSET		0x116
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD3_OFFSET		0x117
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD4_OFFSET		0x118
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD5_OFFSET		0x119
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD6_OFFSET		0x11a
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD7_OFFSET		0x11b
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD8_OFFSET		0x11c
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD9_OFFSET		0x11d
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD10_OFFSET		0x11e
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD11_OFFSET		0x11f
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD12_OFFSET		0x120
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD13_OFFSET		0x121
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD14_OFFSET		0x122
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD15_OFFSET		0x123
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD16_OFFSET		0x124
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD17_OFFSET		0x125
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD18_OFFSET		0x126
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD19_OFFSET		0x127
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD20_OFFSET		0x128
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD21_OFFSET		0x129
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD22_OFFSET		0x12a
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD23_OFFSET		0x12b
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD24_OFFSET		0x12c
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD25_OFFSET		0x12d
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD26_OFFSET		0x12e
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD27_OFFSET		0x12f
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD28_OFFSET		0x130
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD29_OFFSET		0x131
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD30_OFFSET		0x132
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD31_OFFSET		0x133
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD32_OFFSET		0x134
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD33_OFFSET		0x135
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD34_OFFSET		0x136
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD35_OFFSET		0x137
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD36_OFFSET		0x138
#define OMAP2430_CONTROL_PADCONF_AD2DMCAD37_OFFSET		0x139
#define OMAP2430_CONTROL_PADCONF_AD2DMWRITE_OFFSET		0x13a
#define OMAP2430_CONTROL_PADCONF_D2DCLK26MI_OFFSET		0x13b
#define OMAP2430_CONTROL_PADCONF_D2DNRESPWRON1_OFFSET		0x13c
#define OMAP2430_CONTROL_PADCONF_D2DNRESWARM_OFFSET		0x13d
#define OMAP2430_CONTROL_PADCONF_D2DARM9NIRQ_OFFSET		0x13e
#define OMAP2430_CONTROL_PADCONF_D2DUMA2P6FIQ_OFFSET		0x13f
#define OMAP2430_CONTROL_PADCONF_D2DSPINT_OFFSET		0x140
#define OMAP2430_CONTROL_PADCONF_D2DFRINT_OFFSET		0x141
#define OMAP2430_CONTROL_PADCONF_D2DDMAREQ0_OFFSET		0x142
#define OMAP2430_CONTROL_PADCONF_D2DDMAREQ1_OFFSET		0x143
#define OMAP2430_CONTROL_PADCONF_D2DDMAREQ2_OFFSET		0x144
#define OMAP2430_CONTROL_PADCONF_D2DDMAREQ3_OFFSET		0x145
#define OMAP2430_CONTROL_PADCONF_D2DN3GTRST_OFFSET		0x146
#define OMAP2430_CONTROL_PADCONF_D2DN3GTDI_OFFSET		0x147
#define OMAP2430_CONTROL_PADCONF_D2DN3GTDO_OFFSET		0x148
#define OMAP2430_CONTROL_PADCONF_D2DN3GTMS_OFFSET		0x149
#define OMAP2430_CONTROL_PADCONF_D2DN3GTCK_OFFSET		0x14a
#define OMAP2430_CONTROL_PADCONF_D2DN3GRTCK_OFFSET		0x14b
#define OMAP2430_CONTROL_PADCONF_D2DMSTDBY_OFFSET		0x14c
#define OMAP2430_CONTROL_PADCONF_AD2DSREAD_OFFSET		0x14d
#define OMAP2430_CONTROL_PADCONF_D2DSWAKEUP_OFFSET		0x14e
#define OMAP2430_CONTROL_PADCONF_D2DIDLEREQ_OFFSET		0x14f
#define OMAP2430_CONTROL_PADCONF_D2DIDLEACK_OFFSET		0x150
#define OMAP2430_CONTROL_PADCONF_D2DSPARE0_OFFSET		0x151
#define OMAP2430_CONTROL_PADCONF_AD2DSWRITE_OFFSET		0x152
#define OMAP2430_CONTROL_PADCONF_AD2DMREAD_OFFSET		0x153

#define OMAP2430_CONTROL_PADCONF_MUX_SIZE			\
		(OMAP2430_CONTROL_PADCONF_AD2DMREAD_OFFSET + 0x1)