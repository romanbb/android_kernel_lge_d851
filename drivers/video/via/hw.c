/*
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <linux/via-core.h>
#include <asm/olpc.h>
#include "global.h"
#include "via_clock.h"

static struct pll_limit cle266_pll_limits[] = {
	{19, 19, 4, 0},
	{26, 102, 5, 0},
	{53, 112, 6, 0},
	{41, 100, 7, 0},
	{83, 108, 8, 0},
	{87, 118, 9, 0},
	{95, 115, 12, 0},
	{108, 108, 13, 0},
	{83, 83, 17, 0},
	{67, 98, 20, 0},
	{121, 121, 24, 0},
	{99, 99, 29, 0},
	{33, 33, 3, 1},
	{15, 23, 4, 1},
	{37, 121, 5, 1},
	{82, 82, 6, 1},
	{31, 84, 7, 1},
	{83, 83, 8, 1},
	{76, 127, 9, 1},
	{33, 121, 4, 2},
	{91, 118, 5, 2},
	{83, 109, 6, 2},
	{90, 90, 7, 2},
	{93, 93, 2, 3},
	{53, 53, 3, 3},
	{73, 117, 4, 3},
	{101, 127, 5, 3},
	{99, 99, 7, 3}
};

static struct pll_limit k800_pll_limits[] = {
	{22, 22, 2, 0},
	{28, 28, 3, 0},
	{81, 112, 3, 1},
	{86, 166, 4, 1},
	{109, 153, 5, 1},
	{66, 116, 3, 2},
	{93, 137, 4, 2},
	{117, 208, 5, 2},
	{30, 30, 2, 3},
	{69, 125, 3, 3},
	{89, 161, 4, 3},
	{121, 208, 5, 3},
	{66, 66, 2, 4},
	{85, 85, 3, 4},
	{141, 161, 4, 4},
	{177, 177, 5, 4}
};

static struct pll_limit cx700_pll_limits[] = {
	{98, 98, 3, 1},
	{86, 86, 4, 1},
	{109, 208, 5, 1},
	{68, 68, 2, 2},
	{95, 116, 3, 2},
	{93, 166, 4, 2},
	{110, 206, 5, 2},
	{174, 174, 7, 2},
	{82, 109, 3, 3},
	{117, 161, 4, 3},
	{112, 208, 5, 3},
	{141, 202, 5, 4}
};

static struct pll_limit vx855_pll_limits[] = {
	{86, 86, 4, 1},
	{108, 208, 5, 1},
	{110, 208, 5, 2},
	{83, 112, 3, 3},
	{103, 161, 4, 3},
	{112, 209, 5, 3},
	{142, 161, 4, 4},
	{141, 176, 5, 4}
};

/*                                                                    */
static struct io_reg scaling_parameters[] = {
	{VIACR, CR7A, 0xFF, 0x01},	/*                         */
	{VIACR, CR7B, 0xFF, 0x02},	/*                         */
	{VIACR, CR7C, 0xFF, 0x03},	/*                         */
	{VIACR, CR7D, 0xFF, 0x04},	/*                         */
	{VIACR, CR7E, 0xFF, 0x07},	/*                         */
	{VIACR, CR7F, 0xFF, 0x0A},	/*                         */
	{VIACR, CR80, 0xFF, 0x0D},	/*                         */
	{VIACR, CR81, 0xFF, 0x13},	/*                         */
	{VIACR, CR82, 0xFF, 0x16},	/*                         */
	{VIACR, CR83, 0xFF, 0x19},	/*                          */
	{VIACR, CR84, 0xFF, 0x1C},	/*                          */
	{VIACR, CR85, 0xFF, 0x1D},	/*                          */
	{VIACR, CR86, 0xFF, 0x1E},	/*                          */
	{VIACR, CR87, 0xFF, 0x1F},	/*                          */
};

static struct io_reg common_vga[] = {
	{VIACR, CR07, 0x10, 0x10}, /*                           
                                     
                                       
                                        
                             
                               
                                     
                                        */
	{VIACR, CR08, 0xFF, 0x00}, /*                      
                        */
	{VIACR, CR09, 0xDF, 0x40}, /*                    
                                        
                             
                       */
	{VIACR, CR0A, 0xFF, 0x1E}, /*                   
                        */
	{VIACR, CR0B, 0xFF, 0x00}, /*                 
                       */
	{VIACR, CR0E, 0xFF, 0x00}, /*                              */
	{VIACR, CR0F, 0xFF, 0x00}, /*                             */
	{VIACR, CR11, 0xF0, 0x80}, /*                           
                                 
                                      */
	{VIACR, CR14, 0xFF, 0x00}, /*                         
                                         
                                */
	{VIACR, CR17, 0xFF, 0x63}, /*                                       
                                    
                                         
                     
                         
                     */
	{VIACR, CR18, 0xFF, 0xFF}, /*                    */
};

static struct fifo_depth_select display_fifo_depth_reg = {
	/*                        */
	{IGA1_FIFO_DEPTH_SELECT_REG_NUM, {{SR17, 0, 7} } },
	/*                        */
	{IGA2_FIFO_DEPTH_SELECT_REG_NUM,
	 {{CR68, 4, 7}, {CR94, 7, 7}, {CR95, 7, 7} } }
};

static struct fifo_threshold_select fifo_threshold_select_reg = {
	/*                            */
	{IGA1_FIFO_THRESHOLD_REG_NUM, {{SR16, 0, 5}, {SR16, 7, 7} } },
	/*                            */
	{IGA2_FIFO_THRESHOLD_REG_NUM, {{CR68, 0, 3}, {CR95, 4, 6} } }
};

static struct fifo_high_threshold_select fifo_high_threshold_select_reg = {
	/*                                 */
	{IGA1_FIFO_HIGH_THRESHOLD_REG_NUM, {{SR18, 0, 5}, {SR18, 7, 7} } },
	/*                                 */
	{IGA2_FIFO_HIGH_THRESHOLD_REG_NUM, {{CR92, 0, 3}, {CR95, 0, 2} } }
};

static struct display_queue_expire_num display_queue_expire_num_reg = {
	/*                               */
	{IGA1_DISPLAY_QUEUE_EXPIRE_NUM_REG_NUM, {{SR22, 0, 4} } },
	/*                               */
	{IGA2_DISPLAY_QUEUE_EXPIRE_NUM_REG_NUM, {{CR94, 0, 6} } }
};

/*                                 */
static struct fetch_count fetch_count_reg = {
	/*                           */
	{IGA1_FETCH_COUNT_REG_NUM, {{SR1C, 0, 7}, {SR1D, 0, 1} } },
	/*                           */
	{IGA2_FETCH_COUNT_REG_NUM, {{CR65, 0, 7}, {CR67, 2, 3} } }
};

static struct rgbLUT palLUT_table[] = {
	/*         */
	/*                 */
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x2A}, {0x00, 0x2A, 0x00}, {0x00,
								     0x2A,
								     0x2A},
	/*                 */
	{0x2A, 0x00, 0x00}, {0x2A, 0x00, 0x2A}, {0x2A, 0x15, 0x00}, {0x2A,
								     0x2A,
								     0x2A},
	/*                 */
	{0x15, 0x15, 0x15}, {0x15, 0x15, 0x3F}, {0x15, 0x3F, 0x15}, {0x15,
								     0x3F,
								     0x3F},
	/*                 */
	{0x3F, 0x15, 0x15}, {0x3F, 0x15, 0x3F}, {0x3F, 0x3F, 0x15}, {0x3F,
								     0x3F,
								     0x3F},
	/*                 */
	{0x00, 0x00, 0x00}, {0x05, 0x05, 0x05}, {0x08, 0x08, 0x08}, {0x0B,
								     0x0B,
								     0x0B},
	/*                 */
	{0x0E, 0x0E, 0x0E}, {0x11, 0x11, 0x11}, {0x14, 0x14, 0x14}, {0x18,
								     0x18,
								     0x18},
	/*                 */
	{0x1C, 0x1C, 0x1C}, {0x20, 0x20, 0x20}, {0x24, 0x24, 0x24}, {0x28,
								     0x28,
								     0x28},
	/*                 */
	{0x2D, 0x2D, 0x2D}, {0x32, 0x32, 0x32}, {0x38, 0x38, 0x38}, {0x3F,
								     0x3F,
								     0x3F},
	/*                 */
	{0x00, 0x00, 0x3F}, {0x10, 0x00, 0x3F}, {0x1F, 0x00, 0x3F}, {0x2F,
								     0x00,
								     0x3F},
	/*                 */
	{0x3F, 0x00, 0x3F}, {0x3F, 0x00, 0x2F}, {0x3F, 0x00, 0x1F}, {0x3F,
								     0x00,
								     0x10},
	/*                 */
	{0x3F, 0x00, 0x00}, {0x3F, 0x10, 0x00}, {0x3F, 0x1F, 0x00}, {0x3F,
								     0x2F,
								     0x00},
	/*                 */
	{0x3F, 0x3F, 0x00}, {0x2F, 0x3F, 0x00}, {0x1F, 0x3F, 0x00}, {0x10,
								     0x3F,
								     0x00},
	/*                 */
	{0x00, 0x3F, 0x00}, {0x00, 0x3F, 0x10}, {0x00, 0x3F, 0x1F}, {0x00,
								     0x3F,
								     0x2F},
	/*                 */
	{0x00, 0x3F, 0x3F}, {0x00, 0x2F, 0x3F}, {0x00, 0x1F, 0x3F}, {0x00,
								     0x10,
								     0x3F},
	/*                 */
	{0x1F, 0x1F, 0x3F}, {0x27, 0x1F, 0x3F}, {0x2F, 0x1F, 0x3F}, {0x37,
								     0x1F,
								     0x3F},
	/*                 */
	{0x3F, 0x1F, 0x3F}, {0x3F, 0x1F, 0x37}, {0x3F, 0x1F, 0x2F}, {0x3F,
								     0x1F,
								     0x27},
	/*                 */
	{0x3F, 0x1F, 0x1F}, {0x3F, 0x27, 0x1F}, {0x3F, 0x2F, 0x1F}, {0x3F,
								     0x3F,
								     0x1F},
	/*                 */
	{0x3F, 0x3F, 0x1F}, {0x37, 0x3F, 0x1F}, {0x2F, 0x3F, 0x1F}, {0x27,
								     0x3F,
								     0x1F},
	/*                 */
	{0x1F, 0x3F, 0x1F}, {0x1F, 0x3F, 0x27}, {0x1F, 0x3F, 0x2F}, {0x1F,
								     0x3F,
								     0x37},
	/*                 */
	{0x1F, 0x3F, 0x3F}, {0x1F, 0x37, 0x3F}, {0x1F, 0x2F, 0x3F}, {0x1F,
								     0x27,
								     0x3F},
	/*                 */
	{0x2D, 0x2D, 0x3F}, {0x31, 0x2D, 0x3F}, {0x36, 0x2D, 0x3F}, {0x3A,
								     0x2D,
								     0x3F},
	/*                 */
	{0x3F, 0x2D, 0x3F}, {0x3F, 0x2D, 0x3A}, {0x3F, 0x2D, 0x36}, {0x3F,
								     0x2D,
								     0x31},
	/*                 */
	{0x3F, 0x2D, 0x2D}, {0x3F, 0x31, 0x2D}, {0x3F, 0x36, 0x2D}, {0x3F,
								     0x3A,
								     0x2D},
	/*                 */
	{0x3F, 0x3F, 0x2D}, {0x3A, 0x3F, 0x2D}, {0x36, 0x3F, 0x2D}, {0x31,
								     0x3F,
								     0x2D},
	/*                 */
	{0x2D, 0x3F, 0x2D}, {0x2D, 0x3F, 0x31}, {0x2D, 0x3F, 0x36}, {0x2D,
								     0x3F,
								     0x3A},
	/*                 */
	{0x2D, 0x3F, 0x3F}, {0x2D, 0x3A, 0x3F}, {0x2D, 0x36, 0x3F}, {0x2D,
								     0x31,
								     0x3F},
	/*                 */
	{0x00, 0x00, 0x1C}, {0x07, 0x00, 0x1C}, {0x0E, 0x00, 0x1C}, {0x15,
								     0x00,
								     0x1C},
	/*                 */
	{0x1C, 0x00, 0x1C}, {0x1C, 0x00, 0x15}, {0x1C, 0x00, 0x0E}, {0x1C,
								     0x00,
								     0x07},
	/*                 */
	{0x1C, 0x00, 0x00}, {0x1C, 0x07, 0x00}, {0x1C, 0x0E, 0x00}, {0x1C,
								     0x15,
								     0x00},
	/*                 */
	{0x1C, 0x1C, 0x00}, {0x15, 0x1C, 0x00}, {0x0E, 0x1C, 0x00}, {0x07,
								     0x1C,
								     0x00},
	/*                 */
	{0x00, 0x1C, 0x00}, {0x00, 0x1C, 0x07}, {0x00, 0x1C, 0x0E}, {0x00,
								     0x1C,
								     0x15},
	/*                 */
	{0x00, 0x1C, 0x1C}, {0x00, 0x15, 0x1C}, {0x00, 0x0E, 0x1C}, {0x00,
								     0x07,
								     0x1C},
	/*                 */
	{0x0E, 0x0E, 0x1C}, {0x11, 0x0E, 0x1C}, {0x15, 0x0E, 0x1C}, {0x18,
								     0x0E,
								     0x1C},
	/*                 */
	{0x1C, 0x0E, 0x1C}, {0x1C, 0x0E, 0x18}, {0x1C, 0x0E, 0x15}, {0x1C,
								     0x0E,
								     0x11},
	/*                 */
	{0x1C, 0x0E, 0x0E}, {0x1C, 0x11, 0x0E}, {0x1C, 0x15, 0x0E}, {0x1C,
								     0x18,
								     0x0E},
	/*                 */
	{0x1C, 0x1C, 0x0E}, {0x18, 0x1C, 0x0E}, {0x15, 0x1C, 0x0E}, {0x11,
								     0x1C,
								     0x0E},
	/*                 */
	{0x0E, 0x1C, 0x0E}, {0x0E, 0x1C, 0x11}, {0x0E, 0x1C, 0x15}, {0x0E,
								     0x1C,
								     0x18},
	/*                 */
	{0x0E, 0x1C, 0x1C}, {0x0E, 0x18, 0x1C}, {0x0E, 0x15, 0x1C}, {0x0E,
								     0x11,
								     0x1C},
	/*                 */
	{0x14, 0x14, 0x1C}, {0x16, 0x14, 0x1C}, {0x18, 0x14, 0x1C}, {0x1A,
								     0x14,
								     0x1C},
	/*                 */
	{0x1C, 0x14, 0x1C}, {0x1C, 0x14, 0x1A}, {0x1C, 0x14, 0x18}, {0x1C,
								     0x14,
								     0x16},
	/*                 */
	{0x1C, 0x14, 0x14}, {0x1C, 0x16, 0x14}, {0x1C, 0x18, 0x14}, {0x1C,
								     0x1A,
								     0x14},
	/*                 */
	{0x1C, 0x1C, 0x14}, {0x1A, 0x1C, 0x14}, {0x18, 0x1C, 0x14}, {0x16,
								     0x1C,
								     0x14},
	/*                 */
	{0x14, 0x1C, 0x14}, {0x14, 0x1C, 0x16}, {0x14, 0x1C, 0x18}, {0x14,
								     0x1C,
								     0x1A},
	/*                 */
	{0x14, 0x1C, 0x1C}, {0x14, 0x1A, 0x1C}, {0x14, 0x18, 0x1C}, {0x14,
								     0x16,
								     0x1C},
	/*                 */
	{0x00, 0x00, 0x10}, {0x04, 0x00, 0x10}, {0x08, 0x00, 0x10}, {0x0C,
								     0x00,
								     0x10},
	/*                 */
	{0x10, 0x00, 0x10}, {0x10, 0x00, 0x0C}, {0x10, 0x00, 0x08}, {0x10,
								     0x00,
								     0x04},
	/*                 */
	{0x10, 0x00, 0x00}, {0x10, 0x04, 0x00}, {0x10, 0x08, 0x00}, {0x10,
								     0x0C,
								     0x00},
	/*                 */
	{0x10, 0x10, 0x00}, {0x0C, 0x10, 0x00}, {0x08, 0x10, 0x00}, {0x04,
								     0x10,
								     0x00},
	/*                 */
	{0x00, 0x10, 0x00}, {0x00, 0x10, 0x04}, {0x00, 0x10, 0x08}, {0x00,
								     0x10,
								     0x0C},
	/*                 */
	{0x00, 0x10, 0x10}, {0x00, 0x0C, 0x10}, {0x00, 0x08, 0x10}, {0x00,
								     0x04,
								     0x10},
	/*                 */
	{0x08, 0x08, 0x10}, {0x0A, 0x08, 0x10}, {0x0C, 0x08, 0x10}, {0x0E,
								     0x08,
								     0x10},
	/*                 */
	{0x10, 0x08, 0x10}, {0x10, 0x08, 0x0E}, {0x10, 0x08, 0x0C}, {0x10,
								     0x08,
								     0x0A},
	/*                 */
	{0x10, 0x08, 0x08}, {0x10, 0x0A, 0x08}, {0x10, 0x0C, 0x08}, {0x10,
								     0x0E,
								     0x08},
	/*                 */
	{0x10, 0x10, 0x08}, {0x0E, 0x10, 0x08}, {0x0C, 0x10, 0x08}, {0x0A,
								     0x10,
								     0x08},
	/*                 */
	{0x08, 0x10, 0x08}, {0x08, 0x10, 0x0A}, {0x08, 0x10, 0x0C}, {0x08,
								     0x10,
								     0x0E},
	/*                 */
	{0x08, 0x10, 0x10}, {0x08, 0x0E, 0x10}, {0x08, 0x0C, 0x10}, {0x08,
								     0x0A,
								     0x10},
	/*                 */
	{0x0B, 0x0B, 0x10}, {0x0C, 0x0B, 0x10}, {0x0D, 0x0B, 0x10}, {0x0F,
								     0x0B,
								     0x10},
	/*                 */
	{0x10, 0x0B, 0x10}, {0x10, 0x0B, 0x0F}, {0x10, 0x0B, 0x0D}, {0x10,
								     0x0B,
								     0x0C},
	/*                 */
	{0x10, 0x0B, 0x0B}, {0x10, 0x0C, 0x0B}, {0x10, 0x0D, 0x0B}, {0x10,
								     0x0F,
								     0x0B},
	/*                 */
	{0x10, 0x10, 0x0B}, {0x0F, 0x10, 0x0B}, {0x0D, 0x10, 0x0B}, {0x0C,
								     0x10,
								     0x0B},
	/*                 */
	{0x0B, 0x10, 0x0B}, {0x0B, 0x10, 0x0C}, {0x0B, 0x10, 0x0D}, {0x0B,
								     0x10,
								     0x0F},
	/*                 */
	{0x0B, 0x10, 0x10}, {0x0B, 0x0F, 0x10}, {0x0B, 0x0D, 0x10}, {0x0B,
								     0x0C,
								     0x10},
	/*                 */
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00,
								     0x00,
								     0x00},
	/*                 */
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00,
								     0x00,
								     0x00}
};

static struct via_device_mapping device_mapping[] = {
	{VIA_LDVP0, "LDVP0"},
	{VIA_LDVP1, "LDVP1"},
	{VIA_DVP0, "DVP0"},
	{VIA_CRT, "CRT"},
	{VIA_DVP1, "DVP1"},
	{VIA_LVDS1, "LVDS1"},
	{VIA_LVDS2, "LVDS2"}
};

/*                                                           */
static struct via_clock clock;

static void load_fix_bit_crtc_reg(void);
static void __devinit init_gfx_chip_info(int chip_type);
static void __devinit init_tmds_chip_info(void);
static void __devinit init_lvds_chip_info(void);
static void device_screen_off(void);
static void device_screen_on(void);
static void set_display_channel(void);
static void device_off(void);
static void device_on(void);
static void enable_second_display_channel(void);
static void disable_second_display_channel(void);

void viafb_lock_crt(void)
{
	viafb_write_reg_mask(CR11, VIACR, BIT7, BIT7);
}

void viafb_unlock_crt(void)
{
	viafb_write_reg_mask(CR11, VIACR, 0, BIT7);
	viafb_write_reg_mask(CR47, VIACR, 0, BIT0);
}

static void write_dac_reg(u8 index, u8 r, u8 g, u8 b)
{
	outb(index, LUT_INDEX_WRITE);
	outb(r, LUT_DATA);
	outb(g, LUT_DATA);
	outb(b, LUT_DATA);
}

static u32 get_dvi_devices(int output_interface)
{
	switch (output_interface) {
	case INTERFACE_DVP0:
		return VIA_DVP0 | VIA_LDVP0;

	case INTERFACE_DVP1:
		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266)
			return VIA_LDVP1;
		else
			return VIA_DVP1;

	case INTERFACE_DFP_HIGH:
		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266)
			return 0;
		else
			return VIA_LVDS2 | VIA_DVP0;

	case INTERFACE_DFP_LOW:
		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266)
			return 0;
		else
			return VIA_DVP1 | VIA_LVDS1;

	case INTERFACE_TMDS:
		return VIA_LVDS1;
	}

	return 0;
}

static u32 get_lcd_devices(int output_interface)
{
	switch (output_interface) {
	case INTERFACE_DVP0:
		return VIA_DVP0;

	case INTERFACE_DVP1:
		return VIA_DVP1;

	case INTERFACE_DFP_HIGH:
		return VIA_LVDS2 | VIA_DVP0;

	case INTERFACE_DFP_LOW:
		return VIA_LVDS1 | VIA_DVP1;

	case INTERFACE_DFP:
		return VIA_LVDS1 | VIA_LVDS2;

	case INTERFACE_LVDS0:
	case INTERFACE_LVDS0LVDS1:
		return VIA_LVDS1;

	case INTERFACE_LVDS1:
		return VIA_LVDS2;
	}

	return 0;
}

/*                            */
void viafb_set_iga_path(void)
{
	int crt_iga_path = 0;

	if (viafb_SAMM_ON == 1) {
		if (viafb_CRT_ON) {
			if (viafb_primary_dev == CRT_Device)
				crt_iga_path = IGA1;
			else
				crt_iga_path = IGA2;
		}

		if (viafb_DVI_ON) {
			if (viafb_primary_dev == DVI_Device)
				viaparinfo->tmds_setting_info->iga_path = IGA1;
			else
				viaparinfo->tmds_setting_info->iga_path = IGA2;
		}

		if (viafb_LCD_ON) {
			if (viafb_primary_dev == LCD_Device) {
				if (viafb_dual_fb &&
					(viaparinfo->chip_info->gfx_chip_name ==
					UNICHROME_CLE266)) {
					viaparinfo->
					lvds_setting_info->iga_path = IGA2;
					crt_iga_path = IGA1;
					viaparinfo->
					tmds_setting_info->iga_path = IGA1;
				} else
					viaparinfo->
					lvds_setting_info->iga_path = IGA1;
			} else {
				viaparinfo->lvds_setting_info->iga_path = IGA2;
			}
		}
		if (viafb_LCD2_ON) {
			if (LCD2_Device == viafb_primary_dev)
				viaparinfo->lvds_setting_info2->iga_path = IGA1;
			else
				viaparinfo->lvds_setting_info2->iga_path = IGA2;
		}
	} else {
		viafb_SAMM_ON = 0;

		if (viafb_CRT_ON && viafb_LCD_ON) {
			crt_iga_path = IGA1;
			viaparinfo->lvds_setting_info->iga_path = IGA2;
		} else if (viafb_CRT_ON && viafb_DVI_ON) {
			crt_iga_path = IGA1;
			viaparinfo->tmds_setting_info->iga_path = IGA2;
		} else if (viafb_LCD_ON && viafb_DVI_ON) {
			viaparinfo->tmds_setting_info->iga_path = IGA1;
			viaparinfo->lvds_setting_info->iga_path = IGA2;
		} else if (viafb_LCD_ON && viafb_LCD2_ON) {
			viaparinfo->lvds_setting_info->iga_path = IGA2;
			viaparinfo->lvds_setting_info2->iga_path = IGA2;
		} else if (viafb_CRT_ON) {
			crt_iga_path = IGA1;
		} else if (viafb_LCD_ON) {
			viaparinfo->lvds_setting_info->iga_path = IGA2;
		} else if (viafb_DVI_ON) {
			viaparinfo->tmds_setting_info->iga_path = IGA1;
		}
	}

	viaparinfo->shared->iga1_devices = 0;
	viaparinfo->shared->iga2_devices = 0;
	if (viafb_CRT_ON) {
		if (crt_iga_path == IGA1)
			viaparinfo->shared->iga1_devices |= VIA_CRT;
		else
			viaparinfo->shared->iga2_devices |= VIA_CRT;
	}

	if (viafb_DVI_ON) {
		if (viaparinfo->tmds_setting_info->iga_path == IGA1)
			viaparinfo->shared->iga1_devices |= get_dvi_devices(
				viaparinfo->chip_info->
				tmds_chip_info.output_interface);
		else
			viaparinfo->shared->iga2_devices |= get_dvi_devices(
				viaparinfo->chip_info->
				tmds_chip_info.output_interface);
	}

	if (viafb_LCD_ON) {
		if (viaparinfo->lvds_setting_info->iga_path == IGA1)
			viaparinfo->shared->iga1_devices |= get_lcd_devices(
				viaparinfo->chip_info->
				lvds_chip_info.output_interface);
		else
			viaparinfo->shared->iga2_devices |= get_lcd_devices(
				viaparinfo->chip_info->
				lvds_chip_info.output_interface);
	}

	if (viafb_LCD2_ON) {
		if (viaparinfo->lvds_setting_info2->iga_path == IGA1)
			viaparinfo->shared->iga1_devices |= get_lcd_devices(
				viaparinfo->chip_info->
				lvds_chip_info2.output_interface);
		else
			viaparinfo->shared->iga2_devices |= get_lcd_devices(
				viaparinfo->chip_info->
				lvds_chip_info2.output_interface);
	}

	/*                                                             */
	if (machine_is_olpc())
		viaparinfo->shared->iga2_devices = VIA_DVP1 | VIA_LVDS2;
}

static void set_color_register(u8 index, u8 red, u8 green, u8 blue)
{
	outb(0xFF, 0x3C6); /*                     */
	outb(index, 0x3C8);
	outb(red, 0x3C9);
	outb(green, 0x3C9);
	outb(blue, 0x3C9);
}

void viafb_set_primary_color_register(u8 index, u8 red, u8 green, u8 blue)
{
	viafb_write_reg_mask(0x1A, VIASR, 0x00, 0x01);
	set_color_register(index, red, green, blue);
}

void viafb_set_secondary_color_register(u8 index, u8 red, u8 green, u8 blue)
{
	viafb_write_reg_mask(0x1A, VIASR, 0x01, 0x01);
	set_color_register(index, red, green, blue);
}

static void set_source_common(u8 index, u8 offset, u8 iga)
{
	u8 value, mask = 1 << offset;

	switch (iga) {
	case IGA1:
		value = 0x00;
		break;
	case IGA2:
		value = mask;
		break;
	default:
		printk(KERN_WARNING "viafb: Unsupported source: %d\n", iga);
		return;
	}

	via_write_reg_mask(VIACR, index, value, mask);
}

static void set_crt_source(u8 iga)
{
	u8 value;

	switch (iga) {
	case IGA1:
		value = 0x00;
		break;
	case IGA2:
		value = 0x40;
		break;
	default:
		printk(KERN_WARNING "viafb: Unsupported source: %d\n", iga);
		return;
	}

	via_write_reg_mask(VIASR, 0x16, value, 0x40);
}

static inline void set_ldvp0_source(u8 iga)
{
	set_source_common(0x6C, 7, iga);
}

static inline void set_ldvp1_source(u8 iga)
{
	set_source_common(0x93, 7, iga);
}

static inline void set_dvp0_source(u8 iga)
{
	set_source_common(0x96, 4, iga);
}

static inline void set_dvp1_source(u8 iga)
{
	set_source_common(0x9B, 4, iga);
}

static inline void set_lvds1_source(u8 iga)
{
	set_source_common(0x99, 4, iga);
}

static inline void set_lvds2_source(u8 iga)
{
	set_source_common(0x97, 4, iga);
}

void via_set_source(u32 devices, u8 iga)
{
	if (devices & VIA_LDVP0)
		set_ldvp0_source(iga);
	if (devices & VIA_LDVP1)
		set_ldvp1_source(iga);
	if (devices & VIA_DVP0)
		set_dvp0_source(iga);
	if (devices & VIA_CRT)
		set_crt_source(iga);
	if (devices & VIA_DVP1)
		set_dvp1_source(iga);
	if (devices & VIA_LVDS1)
		set_lvds1_source(iga);
	if (devices & VIA_LVDS2)
		set_lvds2_source(iga);
}

static void set_crt_state(u8 state)
{
	u8 value;

	switch (state) {
	case VIA_STATE_ON:
		value = 0x00;
		break;
	case VIA_STATE_STANDBY:
		value = 0x10;
		break;
	case VIA_STATE_SUSPEND:
		value = 0x20;
		break;
	case VIA_STATE_OFF:
		value = 0x30;
		break;
	default:
		return;
	}

	via_write_reg_mask(VIACR, 0x36, value, 0x30);
}

static void set_dvp0_state(u8 state)
{
	u8 value;

	switch (state) {
	case VIA_STATE_ON:
		value = 0xC0;
		break;
	case VIA_STATE_OFF:
		value = 0x00;
		break;
	default:
		return;
	}

	via_write_reg_mask(VIASR, 0x1E, value, 0xC0);
}

static void set_dvp1_state(u8 state)
{
	u8 value;

	switch (state) {
	case VIA_STATE_ON:
		value = 0x30;
		break;
	case VIA_STATE_OFF:
		value = 0x00;
		break;
	default:
		return;
	}

	via_write_reg_mask(VIASR, 0x1E, value, 0x30);
}

static void set_lvds1_state(u8 state)
{
	u8 value;

	switch (state) {
	case VIA_STATE_ON:
		value = 0x03;
		break;
	case VIA_STATE_OFF:
		value = 0x00;
		break;
	default:
		return;
	}

	via_write_reg_mask(VIASR, 0x2A, value, 0x03);
}

static void set_lvds2_state(u8 state)
{
	u8 value;

	switch (state) {
	case VIA_STATE_ON:
		value = 0x0C;
		break;
	case VIA_STATE_OFF:
		value = 0x00;
		break;
	default:
		return;
	}

	via_write_reg_mask(VIASR, 0x2A, value, 0x0C);
}

void via_set_state(u32 devices, u8 state)
{
	/*
                                                
                         
                         
 */
	if (devices & VIA_DVP0)
		set_dvp0_state(state);
	if (devices & VIA_CRT)
		set_crt_state(state);
	if (devices & VIA_DVP1)
		set_dvp1_state(state);
	if (devices & VIA_LVDS1)
		set_lvds1_state(state);
	if (devices & VIA_LVDS2)
		set_lvds2_state(state);
}

void via_set_sync_polarity(u32 devices, u8 polarity)
{
	if (polarity & ~(VIA_HSYNC_NEGATIVE | VIA_VSYNC_NEGATIVE)) {
		printk(KERN_WARNING "viafb: Unsupported polarity: %d\n",
			polarity);
		return;
	}

	if (devices & VIA_CRT)
		via_write_misc_reg_mask(polarity << 6, 0xC0);
	if (devices & VIA_DVP1)
		via_write_reg_mask(VIACR, 0x9B, polarity << 5, 0x60);
	if (devices & VIA_LVDS1)
		via_write_reg_mask(VIACR, 0x99, polarity << 5, 0x60);
	if (devices & VIA_LVDS2)
		via_write_reg_mask(VIACR, 0x97, polarity << 5, 0x60);
}

u32 via_parse_odev(char *input, char **end)
{
	char *ptr = input;
	u32 odev = 0;
	bool next = true;
	int i, len;

	while (next) {
		next = false;
		for (i = 0; i < ARRAY_SIZE(device_mapping); i++) {
			len = strlen(device_mapping[i].name);
			if (!strncmp(ptr, device_mapping[i].name, len)) {
				odev |= device_mapping[i].device;
				ptr += len;
				if (*ptr == ',') {
					ptr++;
					next = true;
				}
			}
		}
	}

	*end = ptr;
	return odev;
}

void via_odev_to_seq(struct seq_file *m, u32 odev)
{
	int i, count = 0;

	for (i = 0; i < ARRAY_SIZE(device_mapping); i++) {
		if (odev & device_mapping[i].device) {
			if (count > 0)
				seq_putc(m, ',');

			seq_puts(m, device_mapping[i].name);
			count++;
		}
	}

	seq_putc(m, '\n');
}

static void load_fix_bit_crtc_reg(void)
{
	viafb_unlock_crt();

	/*                 */
	viafb_write_reg_mask(CR03, VIACR, 0x80, BIT7);
	/*                                                     */
	viafb_write_reg_mask(CR35, VIACR, 0x10, BIT4);
	/*                                                     */
	viafb_write_reg_mask(CR33, VIACR, 0x06, BIT0 + BIT1 + BIT2);
	/*                                               */

	viafb_lock_crt();

	/*                                  */
	if ((viaparinfo->chip_info->gfx_chip_name == UNICHROME_K800)
		|| (viaparinfo->chip_info->gfx_chip_name == UNICHROME_K8M890))
		viafb_write_reg_mask(CR33, VIACR, 0x08, BIT3);
	if ((viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266)
	    && (viaparinfo->chip_info->gfx_chip_revision == CLE266_REVISION_AX))
		viafb_write_reg_mask(SR1A, VIASR, 0x02, BIT1);

}

void viafb_load_reg(int timing_value, int viafb_load_reg_num,
	struct io_register *reg,
	      int io_type)
{
	int reg_mask;
	int bit_num = 0;
	int data;
	int i, j;
	int shift_next_reg;
	int start_index, end_index, cr_index;
	u16 get_bit;

	for (i = 0; i < viafb_load_reg_num; i++) {
		reg_mask = 0;
		data = 0;
		start_index = reg[i].start_bit;
		end_index = reg[i].end_bit;
		cr_index = reg[i].io_addr;

		shift_next_reg = bit_num;
		for (j = start_index; j <= end_index; j++) {
			/*                                                 */
			reg_mask = reg_mask | (BIT0 << j);
			get_bit = (timing_value & (BIT0 << bit_num));
			data =
			    data | ((get_bit >> shift_next_reg) << start_index);
			bit_num++;
		}
		if (io_type == VIACR)
			viafb_write_reg_mask(cr_index, VIACR, data, reg_mask);
		else
			viafb_write_reg_mask(cr_index, VIASR, data, reg_mask);
	}

}

/*                 */
void viafb_write_regx(struct io_reg RegTable[], int ItemNum)
{
	int i;

	/*                                                     */

	for (i = 0; i < ItemNum; i++)
		via_write_reg_mask(RegTable[i].port, RegTable[i].index,
			RegTable[i].value, RegTable[i].mask);
}

void viafb_load_fetch_count_reg(int h_addr, int bpp_byte, int set_iga)
{
	int reg_value;
	int viafb_load_reg_num;
	struct io_register *reg = NULL;

	switch (set_iga) {
	case IGA1:
		reg_value = IGA1_FETCH_COUNT_FORMULA(h_addr, bpp_byte);
		viafb_load_reg_num = fetch_count_reg.
			iga1_fetch_count_reg.reg_num;
		reg = fetch_count_reg.iga1_fetch_count_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIASR);
		break;
	case IGA2:
		reg_value = IGA2_FETCH_COUNT_FORMULA(h_addr, bpp_byte);
		viafb_load_reg_num = fetch_count_reg.
			iga2_fetch_count_reg.reg_num;
		reg = fetch_count_reg.iga2_fetch_count_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIACR);
		break;
	}

}

void viafb_load_FIFO_reg(int set_iga, int hor_active, int ver_active)
{
	int reg_value;
	int viafb_load_reg_num;
	struct io_register *reg = NULL;
	int iga1_fifo_max_depth = 0, iga1_fifo_threshold =
	    0, iga1_fifo_high_threshold = 0, iga1_display_queue_expire_num = 0;
	int iga2_fifo_max_depth = 0, iga2_fifo_threshold =
	    0, iga2_fifo_high_threshold = 0, iga2_display_queue_expire_num = 0;

	if (set_iga == IGA1) {
		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_K800) {
			iga1_fifo_max_depth = K800_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = K800_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    K800_IGA1_FIFO_HIGH_THRESHOLD;
			/*                                                    
                          */
			if ((hor_active > 1280) && (ver_active > 1024))
				iga1_display_queue_expire_num = 16;
			else
				iga1_display_queue_expire_num =
				    K800_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;

		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_PM800) {
			iga1_fifo_max_depth = P880_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = P880_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    P880_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    P880_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;

			/*                                                    
                          */
			if ((hor_active > 1280) && (ver_active > 1024))
				iga1_display_queue_expire_num = 16;
			else
				iga1_display_queue_expire_num =
				    P880_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CN700) {
			iga1_fifo_max_depth = CN700_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = CN700_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    CN700_IGA1_FIFO_HIGH_THRESHOLD;

			/*                                               
                               */
			if ((hor_active > 1280) && (ver_active > 1024))
				iga1_display_queue_expire_num = 16;
			else
				iga1_display_queue_expire_num =
				    CN700_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700) {
			iga1_fifo_max_depth = CX700_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = CX700_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    CX700_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    CX700_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_K8M890) {
			iga1_fifo_max_depth = K8M890_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = K8M890_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    K8M890_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    K8M890_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_P4M890) {
			iga1_fifo_max_depth = P4M890_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = P4M890_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    P4M890_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    P4M890_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_P4M900) {
			iga1_fifo_max_depth = P4M900_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = P4M900_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    P4M900_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    P4M900_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_VX800) {
			iga1_fifo_max_depth = VX800_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = VX800_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    VX800_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    VX800_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_VX855) {
			iga1_fifo_max_depth = VX855_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = VX855_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    VX855_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    VX855_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_VX900) {
			iga1_fifo_max_depth = VX900_IGA1_FIFO_MAX_DEPTH;
			iga1_fifo_threshold = VX900_IGA1_FIFO_THRESHOLD;
			iga1_fifo_high_threshold =
			    VX900_IGA1_FIFO_HIGH_THRESHOLD;
			iga1_display_queue_expire_num =
			    VX900_IGA1_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		/*                                */
		reg_value = IGA1_FIFO_DEPTH_SELECT_FORMULA(iga1_fifo_max_depth);
		viafb_load_reg_num =
		    display_fifo_depth_reg.iga1_fifo_depth_select_reg.reg_num;
		reg = display_fifo_depth_reg.iga1_fifo_depth_select_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIASR);

		/*                                   */
		reg_value = IGA1_FIFO_THRESHOLD_FORMULA(iga1_fifo_threshold);
		viafb_load_reg_num =
		    fifo_threshold_select_reg.
		    iga1_fifo_threshold_select_reg.reg_num;
		reg =
		    fifo_threshold_select_reg.
		    iga1_fifo_threshold_select_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIASR);

		/*                                */
		reg_value =
		    IGA1_FIFO_HIGH_THRESHOLD_FORMULA(iga1_fifo_high_threshold);
		viafb_load_reg_num =
		    fifo_high_threshold_select_reg.
		    iga1_fifo_high_threshold_select_reg.reg_num;
		reg =
		    fifo_high_threshold_select_reg.
		    iga1_fifo_high_threshold_select_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIASR);

		/*                              */
		reg_value =
		    IGA1_DISPLAY_QUEUE_EXPIRE_NUM_FORMULA
		    (iga1_display_queue_expire_num);
		viafb_load_reg_num =
		    display_queue_expire_num_reg.
		    iga1_display_queue_expire_num_reg.reg_num;
		reg =
		    display_queue_expire_num_reg.
		    iga1_display_queue_expire_num_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIASR);

	} else {
		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_K800) {
			iga2_fifo_max_depth = K800_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = K800_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    K800_IGA2_FIFO_HIGH_THRESHOLD;

			/*                                               
                                */
			if ((hor_active > 1280) && (ver_active > 1024))
				iga2_display_queue_expire_num = 16;
			else
				iga2_display_queue_expire_num =
				    K800_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_PM800) {
			iga2_fifo_max_depth = P880_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = P880_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    P880_IGA2_FIFO_HIGH_THRESHOLD;

			/*                                               
                                */
			if ((hor_active > 1280) && (ver_active > 1024))
				iga2_display_queue_expire_num = 16;
			else
				iga2_display_queue_expire_num =
				    P880_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CN700) {
			iga2_fifo_max_depth = CN700_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = CN700_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    CN700_IGA2_FIFO_HIGH_THRESHOLD;

			/*                                               
                               */
			if ((hor_active > 1280) && (ver_active > 1024))
				iga2_display_queue_expire_num = 16;
			else
				iga2_display_queue_expire_num =
				    CN700_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700) {
			iga2_fifo_max_depth = CX700_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = CX700_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    CX700_IGA2_FIFO_HIGH_THRESHOLD;
			iga2_display_queue_expire_num =
			    CX700_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_K8M890) {
			iga2_fifo_max_depth = K8M890_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = K8M890_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    K8M890_IGA2_FIFO_HIGH_THRESHOLD;
			iga2_display_queue_expire_num =
			    K8M890_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_P4M890) {
			iga2_fifo_max_depth = P4M890_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = P4M890_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    P4M890_IGA2_FIFO_HIGH_THRESHOLD;
			iga2_display_queue_expire_num =
			    P4M890_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_P4M900) {
			iga2_fifo_max_depth = P4M900_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = P4M900_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    P4M900_IGA2_FIFO_HIGH_THRESHOLD;
			iga2_display_queue_expire_num =
			    P4M900_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_VX800) {
			iga2_fifo_max_depth = VX800_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = VX800_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    VX800_IGA2_FIFO_HIGH_THRESHOLD;
			iga2_display_queue_expire_num =
			    VX800_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_VX855) {
			iga2_fifo_max_depth = VX855_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = VX855_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    VX855_IGA2_FIFO_HIGH_THRESHOLD;
			iga2_display_queue_expire_num =
			    VX855_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_VX900) {
			iga2_fifo_max_depth = VX900_IGA2_FIFO_MAX_DEPTH;
			iga2_fifo_threshold = VX900_IGA2_FIFO_THRESHOLD;
			iga2_fifo_high_threshold =
			    VX900_IGA2_FIFO_HIGH_THRESHOLD;
			iga2_display_queue_expire_num =
			    VX900_IGA2_DISPLAY_QUEUE_EXPIRE_NUM;
		}

		if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_K800) {
			/*                                */
			reg_value =
			    IGA2_FIFO_DEPTH_SELECT_FORMULA(iga2_fifo_max_depth)
			    - 1;
			/*                        */
			viafb_load_reg_num =
			    display_fifo_depth_reg.
			    iga2_fifo_depth_select_reg.reg_num;
			reg =
			    display_fifo_depth_reg.
			    iga2_fifo_depth_select_reg.reg;
			viafb_load_reg(reg_value,
				viafb_load_reg_num, reg, VIACR);
		} else {

			/*                                */
			reg_value =
			    IGA2_FIFO_DEPTH_SELECT_FORMULA(iga2_fifo_max_depth);
			viafb_load_reg_num =
			    display_fifo_depth_reg.
			    iga2_fifo_depth_select_reg.reg_num;
			reg =
			    display_fifo_depth_reg.
			    iga2_fifo_depth_select_reg.reg;
			viafb_load_reg(reg_value,
				viafb_load_reg_num, reg, VIACR);
		}

		/*                                   */
		reg_value = IGA2_FIFO_THRESHOLD_FORMULA(iga2_fifo_threshold);
		viafb_load_reg_num =
		    fifo_threshold_select_reg.
		    iga2_fifo_threshold_select_reg.reg_num;
		reg =
		    fifo_threshold_select_reg.
		    iga2_fifo_threshold_select_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIACR);

		/*                                */
		reg_value =
		    IGA2_FIFO_HIGH_THRESHOLD_FORMULA(iga2_fifo_high_threshold);
		viafb_load_reg_num =
		    fifo_high_threshold_select_reg.
		    iga2_fifo_high_threshold_select_reg.reg_num;
		reg =
		    fifo_high_threshold_select_reg.
		    iga2_fifo_high_threshold_select_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIACR);

		/*                              */
		reg_value =
		    IGA2_DISPLAY_QUEUE_EXPIRE_NUM_FORMULA
		    (iga2_display_queue_expire_num);
		viafb_load_reg_num =
		    display_queue_expire_num_reg.
		    iga2_display_queue_expire_num_reg.reg_num;
		reg =
		    display_queue_expire_num_reg.
		    iga2_display_queue_expire_num_reg.reg;
		viafb_load_reg(reg_value, viafb_load_reg_num, reg, VIACR);

	}

}

static struct via_pll_config get_pll_config(struct pll_limit *limits, int size,
	int clk)
{
	struct via_pll_config cur, up, down, best = {0, 1, 0};
	const u32 f0 = 14318180; /*              */
	int i, f;

	for (i = 0; i < size; i++) {
		cur.rshift = limits[i].rshift;
		cur.divisor = limits[i].divisor;
		cur.multiplier = clk / ((f0 / cur.divisor)>>cur.rshift);
		f = abs(get_pll_output_frequency(f0, cur) - clk);
		up = down = cur;
		up.multiplier++;
		down.multiplier--;
		if (abs(get_pll_output_frequency(f0, up) - clk) < f)
			cur = up;
		else if (abs(get_pll_output_frequency(f0, down) - clk) < f)
			cur = down;

		if (cur.multiplier < limits[i].multiplier_min)
			cur.multiplier = limits[i].multiplier_min;
		else if (cur.multiplier > limits[i].multiplier_max)
			cur.multiplier = limits[i].multiplier_max;

		f = abs(get_pll_output_frequency(f0, cur) - clk);
		if (f < abs(get_pll_output_frequency(f0, best) - clk))
			best = cur;
	}

	return best;
}

static struct via_pll_config get_best_pll_config(int clk)
{
	struct via_pll_config config;

	switch (viaparinfo->chip_info->gfx_chip_name) {
	case UNICHROME_CLE266:
	case UNICHROME_K400:
		config = get_pll_config(cle266_pll_limits,
			ARRAY_SIZE(cle266_pll_limits), clk);
		break;
	case UNICHROME_K800:
	case UNICHROME_PM800:
	case UNICHROME_CN700:
		config = get_pll_config(k800_pll_limits,
			ARRAY_SIZE(k800_pll_limits), clk);
		break;
	case UNICHROME_CX700:
	case UNICHROME_CN750:
	case UNICHROME_K8M890:
	case UNICHROME_P4M890:
	case UNICHROME_P4M900:
	case UNICHROME_VX800:
		config = get_pll_config(cx700_pll_limits,
			ARRAY_SIZE(cx700_pll_limits), clk);
		break;
	case UNICHROME_VX855:
	case UNICHROME_VX900:
		config = get_pll_config(vx855_pll_limits,
			ARRAY_SIZE(vx855_pll_limits), clk);
		break;
	}

	return config;
}

/*         */
void viafb_set_vclock(u32 clk, int set_iga)
{
	struct via_pll_config config = get_best_pll_config(clk);

	if (set_iga == IGA1)
		clock.set_primary_pll(config);
	if (set_iga == IGA2)
		clock.set_secondary_pll(config);

	/*       */
	via_write_misc_reg_mask(0x0C, 0x0C); /*                       */
}

struct display_timing var_to_timing(const struct fb_var_screeninfo *var,
	u16 cxres, u16 cyres)
{
	struct display_timing timing;
	u16 dx = (var->xres - cxres) / 2, dy = (var->yres - cyres) / 2;

	timing.hor_addr = cxres;
	timing.hor_sync_start = timing.hor_addr + var->right_margin + dx;
	timing.hor_sync_end = timing.hor_sync_start + var->hsync_len;
	timing.hor_total = timing.hor_sync_end + var->left_margin + dx;
	timing.hor_blank_start = timing.hor_addr + dx;
	timing.hor_blank_end = timing.hor_total - dx;
	timing.ver_addr = cyres;
	timing.ver_sync_start = timing.ver_addr + var->lower_margin + dy;
	timing.ver_sync_end = timing.ver_sync_start + var->vsync_len;
	timing.ver_total = timing.ver_sync_end + var->upper_margin + dy;
	timing.ver_blank_start = timing.ver_addr + dy;
	timing.ver_blank_end = timing.ver_total - dy;
	return timing;
}

void viafb_fill_crtc_timing(const struct fb_var_screeninfo *var,
	u16 cxres, u16 cyres, int iga)
{
	struct display_timing crt_reg = var_to_timing(var,
		cxres ? cxres : var->xres, cyres ? cyres : var->yres);

	if (iga == IGA1)
		via_set_primary_timing(&crt_reg);
	else if (iga == IGA2)
		via_set_secondary_timing(&crt_reg);

	viafb_load_fetch_count_reg(var->xres, var->bits_per_pixel / 8, iga);
	if (viaparinfo->chip_info->gfx_chip_name != UNICHROME_CLE266
		&& viaparinfo->chip_info->gfx_chip_name != UNICHROME_K400)
		viafb_load_FIFO_reg(iga, var->xres, var->yres);

	viafb_set_vclock(PICOS2KHZ(var->pixclock) * 1000, iga);
}

void __devinit viafb_init_chip_info(int chip_type)
{
	via_clock_init(&clock, chip_type);
	init_gfx_chip_info(chip_type);
	init_tmds_chip_info();
	init_lvds_chip_info();

	/*                             */
	viafb_set_iga_path();

	viaparinfo->lvds_setting_info->display_method = viafb_lcd_dsp_method;
	viaparinfo->lvds_setting_info->lcd_mode = viafb_lcd_mode;
	viaparinfo->lvds_setting_info2->display_method =
		viaparinfo->lvds_setting_info->display_method;
	viaparinfo->lvds_setting_info2->lcd_mode =
		viaparinfo->lvds_setting_info->lcd_mode;
}

void viafb_update_device_setting(int hres, int vres, int bpp, int flag)
{
	if (flag == 0) {
		viaparinfo->tmds_setting_info->h_active = hres;
		viaparinfo->tmds_setting_info->v_active = vres;
	} else {

		if (viaparinfo->tmds_setting_info->iga_path == IGA2) {
			viaparinfo->tmds_setting_info->h_active = hres;
			viaparinfo->tmds_setting_info->v_active = vres;
		}

	}
}

static void __devinit init_gfx_chip_info(int chip_type)
{
	u8 tmp;

	viaparinfo->chip_info->gfx_chip_name = chip_type;

	/*                               */
	if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266) {
		/*                                    */
		tmp = viafb_read_reg(VIACR, CR4F);
		viafb_write_reg(CR4F, VIACR, 0x55);
		if (viafb_read_reg(VIACR, CR4F) != 0x55)
			viaparinfo->chip_info->gfx_chip_revision =
			CLE266_REVISION_AX;
		else
			viaparinfo->chip_info->gfx_chip_revision =
			CLE266_REVISION_CX;
		/*                            */
		viafb_write_reg(CR4F, VIACR, tmp);
	}

	if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700) {
		tmp = viafb_read_reg(VIASR, SR43);
		DEBUG_MSG(KERN_INFO "SR43:%X\n", tmp);
		if (tmp & 0x02) {
			viaparinfo->chip_info->gfx_chip_revision =
				CX700_REVISION_700M2;
		} else if (tmp & 0x40) {
			viaparinfo->chip_info->gfx_chip_revision =
				CX700_REVISION_700M;
		} else {
			viaparinfo->chip_info->gfx_chip_revision =
				CX700_REVISION_700;
		}
	}

	/*                                   */
	switch (viaparinfo->chip_info->gfx_chip_name) {
	case UNICHROME_VX800:
	case UNICHROME_VX855:
	case UNICHROME_VX900:
		viaparinfo->chip_info->twod_engine = VIA_2D_ENG_M1;
		break;
	case UNICHROME_K8M890:
	case UNICHROME_P4M900:
		viaparinfo->chip_info->twod_engine = VIA_2D_ENG_H5;
		break;
	default:
		viaparinfo->chip_info->twod_engine = VIA_2D_ENG_H2;
		break;
	}
}

static void __devinit init_tmds_chip_info(void)
{
	viafb_tmds_trasmitter_identify();

	if (INTERFACE_NONE == viaparinfo->chip_info->tmds_chip_info.
		output_interface) {
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_CX700:
			{
				/*                                            */
				if ((viafb_display_hardware_layout ==
				     HW_LAYOUT_DVI_ONLY)
				    || (viafb_display_hardware_layout ==
					HW_LAYOUT_LCD_DVI)) {
					viaparinfo->chip_info->tmds_chip_info.
					    output_interface = INTERFACE_TMDS;
				} else {
					viaparinfo->chip_info->tmds_chip_info.
						output_interface =
						INTERFACE_NONE;
				}
				break;
			}
		case UNICHROME_K8M890:
		case UNICHROME_P4M900:
		case UNICHROME_P4M890:
			/*                                         */
			viaparinfo->chip_info->tmds_chip_info.output_interface =
			    INTERFACE_DFP_LOW;
			break;
		default:
			{
				/*                          */
				viaparinfo->chip_info->tmds_chip_info
				.output_interface = INTERFACE_DVP1;
			}
		}
	}

	DEBUG_MSG(KERN_INFO "TMDS Chip = %d\n",
		  viaparinfo->chip_info->tmds_chip_info.tmds_chip_name);
	viafb_init_dvi_size(&viaparinfo->shared->chip_info.tmds_chip_info,
		&viaparinfo->shared->tmds_setting_info);
}

static void __devinit init_lvds_chip_info(void)
{
	viafb_lvds_trasmitter_identify();
	viafb_init_lcd_size();
	viafb_init_lvds_output_interface(&viaparinfo->chip_info->lvds_chip_info,
				   viaparinfo->lvds_setting_info);
	if (viaparinfo->chip_info->lvds_chip_info2.lvds_chip_name) {
		viafb_init_lvds_output_interface(&viaparinfo->chip_info->
			lvds_chip_info2, viaparinfo->lvds_setting_info2);
	}
	/*                                            
                                         */
	if ((UNICHROME_CX700 == viaparinfo->chip_info->gfx_chip_name)
	    && (HW_LAYOUT_LCD1_LCD2 == viafb_display_hardware_layout)) {
		if ((INTEGRATED_LVDS == viaparinfo->chip_info->lvds_chip_info.
			lvds_chip_name) && (INTEGRATED_LVDS ==
			viaparinfo->chip_info->
			lvds_chip_info2.lvds_chip_name)) {
			viaparinfo->chip_info->lvds_chip_info.output_interface =
				INTERFACE_LVDS0;
			viaparinfo->chip_info->lvds_chip_info2.
				output_interface =
			    INTERFACE_LVDS1;
		}
	}

	DEBUG_MSG(KERN_INFO "LVDS Chip = %d\n",
		  viaparinfo->chip_info->lvds_chip_info.lvds_chip_name);
	DEBUG_MSG(KERN_INFO "LVDS1 output_interface = %d\n",
		  viaparinfo->chip_info->lvds_chip_info.output_interface);
	DEBUG_MSG(KERN_INFO "LVDS2 output_interface = %d\n",
		  viaparinfo->chip_info->lvds_chip_info.output_interface);
}

void __devinit viafb_init_dac(int set_iga)
{
	int i;
	u8 tmp;

	if (set_iga == IGA1) {
		/*                              */
		viafb_write_reg_mask(SR1A, VIASR, 0x00, BIT0);
		/*              */
		viafb_write_reg_mask(SR1B, VIASR, 0x00, BIT7 + BIT6);
		for (i = 0; i < 256; i++) {
			write_dac_reg(i, palLUT_table[i].red,
				      palLUT_table[i].green,
				      palLUT_table[i].blue);
		}
		/*             */
		viafb_write_reg_mask(SR1B, VIASR, 0xC0, BIT7 + BIT6);
	} else {
		tmp = viafb_read_reg(VIACR, CR6A);
		/*                                */
		viafb_write_reg_mask(CR6A, VIACR, 0x40, BIT6);
		viafb_write_reg_mask(SR1A, VIASR, 0x01, BIT0);
		for (i = 0; i < 256; i++) {
			write_dac_reg(i, palLUT_table[i].red,
				      palLUT_table[i].green,
				      palLUT_table[i].blue);
		}
		/*                          */
		viafb_write_reg_mask(SR1A, VIASR, 0x00, BIT0);
		viafb_write_reg(CR6A, VIACR, tmp);
	}
}

static void device_screen_off(void)
{
	/*                            */
	viafb_write_reg_mask(SR01, VIASR, 0x20, BIT5);
}

static void device_screen_on(void)
{
	/*                           */
	viafb_write_reg_mask(SR01, VIASR, 0x00, BIT5);
}

static void set_display_channel(void)
{
	/*                                                       
                                 */
	if (viafb_LCD2_ON &&
		viaparinfo->lvds_setting_info2->device_lcd_dualedge) {
		/*                       */
		/*                           */
		viafb_write_reg_mask(CRD2, VIACR, 0x20, BIT4 + BIT5);
	} else if (viafb_LCD_ON && viafb_DVI_ON) {
		/*              */
		/*                              */
		viafb_write_reg_mask(CRD2, VIACR, 0x10, BIT4 + BIT5);
	} else if (viafb_DVI_ON) {
		/*                             */
		viafb_write_reg_mask(CRD2, VIACR, 0x30, BIT4 + BIT5);
	} else if (viafb_LCD_ON) {
		if (viaparinfo->lvds_setting_info->device_lcd_dualedge) {
			/*                       */
			/*                           */
			viafb_write_reg_mask(CRD2, VIACR, 0x20, BIT4 + BIT5);
		} else {
			/*                               */
			viafb_write_reg_mask(CRD2, VIACR, 0x00, BIT4 + BIT5);
		}
	}
}

static u8 get_sync(struct fb_var_screeninfo *var)
{
	u8 polarity = 0;

	if (!(var->sync & FB_SYNC_HOR_HIGH_ACT))
		polarity |= VIA_HSYNC_NEGATIVE;
	if (!(var->sync & FB_SYNC_VERT_HIGH_ACT))
		polarity |= VIA_VSYNC_NEGATIVE;
	return polarity;
}

static void hw_init(void)
{
	int i;

	inb(VIAStatus);
	outb(0x00, VIAAR);

	/*                                     */
	viafb_write_regx(common_vga, ARRAY_SIZE(common_vga));
	switch (viaparinfo->chip_info->gfx_chip_name) {
	case UNICHROME_CLE266:
		viafb_write_regx(CLE266_ModeXregs, NUM_TOTAL_CLE266_ModeXregs);
		break;

	case UNICHROME_K400:
		viafb_write_regx(KM400_ModeXregs, NUM_TOTAL_KM400_ModeXregs);
		break;

	case UNICHROME_K800:
	case UNICHROME_PM800:
		viafb_write_regx(CN400_ModeXregs, NUM_TOTAL_CN400_ModeXregs);
		break;

	case UNICHROME_CN700:
	case UNICHROME_K8M890:
	case UNICHROME_P4M890:
	case UNICHROME_P4M900:
		viafb_write_regx(CN700_ModeXregs, NUM_TOTAL_CN700_ModeXregs);
		break;

	case UNICHROME_CX700:
	case UNICHROME_VX800:
		viafb_write_regx(CX700_ModeXregs, NUM_TOTAL_CX700_ModeXregs);
		break;

	case UNICHROME_VX855:
	case UNICHROME_VX900:
		viafb_write_regx(VX855_ModeXregs, NUM_TOTAL_VX855_ModeXregs);
		break;
	}

	/*                                                         */
	via_write_reg_mask(VIACR, 0x45, 0x00, 0x01);

	/*                                                     */
	via_write_reg_mask(VIACR, 0xFD, 0, 0x80); /*                        */
	viafb_write_regx(scaling_parameters, ARRAY_SIZE(scaling_parameters));

	/*                      */
	/*                     */
	outb(VPIT.Misc, VIA_MISC_REG_WRITE);

	/*                 */
	for (i = 1; i <= StdSR; i++)
		via_write_reg(VIASR, i, VPIT.SR[i - 1]);

	viafb_write_reg_mask(0x15, VIASR, 0xA2, 0xA2);

	/*                          */
	for (i = 0; i < StdGR; i++)
		via_write_reg(VIAGR, i, VPIT.GR[i]);

	/*                            */
	for (i = 0; i < StdAR; i++) {
		inb(VIAStatus);
		outb(i, VIAAR);
		outb(VPIT.AR[i], VIAAR);
	}

	inb(VIAStatus);
	outb(0x20, VIAAR);

	load_fix_bit_crtc_reg();
}

int viafb_setmode(void)
{
	int j, cxres = 0, cyres = 0;
	int port;
	u32 devices = viaparinfo->shared->iga1_devices
		| viaparinfo->shared->iga2_devices;
	u8 value, index, mask;
	struct fb_var_screeninfo var2;

	device_screen_off();
	device_off();
	via_set_state(devices, VIA_STATE_OFF);

	hw_init();

	/*                       */

	if ((viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266
		|| viaparinfo->chip_info->gfx_chip_name == UNICHROME_K400)
		&& viafbinfo->var.xres == 1024 && viafbinfo->var.yres == 768) {
		for (j = 0; j < res_patch_table[0].table_length; j++) {
			index = res_patch_table[0].io_reg_table[j].index;
			port = res_patch_table[0].io_reg_table[j].port;
			value = res_patch_table[0].io_reg_table[j].value;
			mask = res_patch_table[0].io_reg_table[j].mask;
			viafb_write_reg_mask(index, port, value, mask);
		}
	}

	via_set_primary_pitch(viafbinfo->fix.line_length);
	via_set_secondary_pitch(viafb_dual_fb ? viafbinfo1->fix.line_length
		: viafbinfo->fix.line_length);
	via_set_primary_color_depth(viaparinfo->depth);
	via_set_secondary_color_depth(viafb_dual_fb ? viaparinfo1->depth
		: viaparinfo->depth);
	via_set_source(viaparinfo->shared->iga1_devices, IGA1);
	via_set_source(viaparinfo->shared->iga2_devices, IGA2);
	if (viaparinfo->shared->iga2_devices)
		enable_second_display_channel();
	else
		disable_second_display_channel();

	/*                             */

	/*                 */

	if (viafb_dual_fb) {
		var2 = viafbinfo1->var;
	} else if (viafb_SAMM_ON) {
		viafb_fill_var_timing_info(&var2, viafb_get_best_mode(
			viafb_second_xres, viafb_second_yres, viafb_refresh1));
		cxres = viafbinfo->var.xres;
		cyres = viafbinfo->var.yres;
		var2.bits_per_pixel = viafbinfo->var.bits_per_pixel;
	}

	/*              */
	if (viafb_CRT_ON) {
		if (viaparinfo->shared->iga2_devices & VIA_CRT
			&& viafb_SAMM_ON)
			viafb_fill_crtc_timing(&var2, cxres, cyres, IGA2);
		else
			viafb_fill_crtc_timing(&viafbinfo->var, 0, 0,
				(viaparinfo->shared->iga1_devices & VIA_CRT)
				? IGA1 : IGA2);

		/*                                                             
                                                          
                           */
		if (viafbinfo->var.xres % 8) {
			viafb_unlock_crt();
			viafb_write_reg(CR02, VIACR,
				viafb_read_reg(VIACR, CR02) - 1);
			viafb_lock_crt();
		}
	}

	if (viafb_DVI_ON) {
		if (viaparinfo->shared->tmds_setting_info.iga_path == IGA2
			&& viafb_SAMM_ON)
			viafb_dvi_set_mode(&var2, cxres, cyres, IGA2);
		else
			viafb_dvi_set_mode(&viafbinfo->var, 0, 0,
				viaparinfo->tmds_setting_info->iga_path);
	}

	if (viafb_LCD_ON) {
		if (viafb_SAMM_ON &&
			(viaparinfo->lvds_setting_info->iga_path == IGA2)) {
			viafb_lcd_set_mode(&var2, cxres, cyres,
				viaparinfo->lvds_setting_info,
				&viaparinfo->chip_info->lvds_chip_info);
		} else {
			/*                                                  */
			if (viaparinfo->lvds_setting_info->iga_path == IGA1) {
				viaparinfo->lvds_setting_info->display_method =
				    LCD_CENTERING;
			}
			viafb_lcd_set_mode(&viafbinfo->var, 0, 0,
				viaparinfo->lvds_setting_info,
				&viaparinfo->chip_info->lvds_chip_info);
		}
	}
	if (viafb_LCD2_ON) {
		if (viafb_SAMM_ON &&
			(viaparinfo->lvds_setting_info2->iga_path == IGA2)) {
			viafb_lcd_set_mode(&var2, cxres, cyres,
				viaparinfo->lvds_setting_info2,
				&viaparinfo->chip_info->lvds_chip_info2);
		} else {
			/*                                                  */
			if (viaparinfo->lvds_setting_info2->iga_path == IGA1) {
				viaparinfo->lvds_setting_info2->display_method =
				    LCD_CENTERING;
			}
			viafb_lcd_set_mode(&viafbinfo->var, 0, 0,
				viaparinfo->lvds_setting_info2,
				&viaparinfo->chip_info->lvds_chip_info2);
		}
	}

	if ((viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700)
	    && (viafb_LCD_ON || viafb_DVI_ON))
		set_display_channel();

	/*                                                                  */
	if (!viafb_hotplug) {
		viafb_hotplug_Xres = viafbinfo->var.xres;
		viafb_hotplug_Yres = viafbinfo->var.yres;
		viafb_hotplug_bpp = viafbinfo->var.bits_per_pixel;
		viafb_hotplug_refresh = viafb_refresh;

		if (viafb_DVI_ON)
			viafb_DeviceStatus = DVI_Device;
		else
			viafb_DeviceStatus = CRT_Device;
	}
	device_on();
	if (!viafb_SAMM_ON)
		via_set_sync_polarity(devices, get_sync(&viafbinfo->var));
	else {
		via_set_sync_polarity(viaparinfo->shared->iga1_devices,
			get_sync(&viafbinfo->var));
		via_set_sync_polarity(viaparinfo->shared->iga2_devices,
			get_sync(&var2));
	}

	clock.set_engine_pll_state(VIA_STATE_ON);
	clock.set_primary_clock_source(VIA_CLKSRC_X1, true);
	clock.set_secondary_clock_source(VIA_CLKSRC_X1, true);

#ifdef CONFIG_FB_VIA_X_COMPATIBILITY
	clock.set_primary_pll_state(VIA_STATE_ON);
	clock.set_primary_clock_state(VIA_STATE_ON);
	clock.set_secondary_pll_state(VIA_STATE_ON);
	clock.set_secondary_clock_state(VIA_STATE_ON);
#else
	if (viaparinfo->shared->iga1_devices) {
		clock.set_primary_pll_state(VIA_STATE_ON);
		clock.set_primary_clock_state(VIA_STATE_ON);
	} else {
		clock.set_primary_pll_state(VIA_STATE_OFF);
		clock.set_primary_clock_state(VIA_STATE_OFF);
	}

	if (viaparinfo->shared->iga2_devices) {
		clock.set_secondary_pll_state(VIA_STATE_ON);
		clock.set_secondary_clock_state(VIA_STATE_ON);
	} else {
		clock.set_secondary_pll_state(VIA_STATE_OFF);
		clock.set_secondary_clock_state(VIA_STATE_OFF);
	}
#endif /*                             */

	via_set_state(devices, VIA_STATE_ON);
	device_screen_on();
	return 1;
}

int viafb_get_refresh(int hres, int vres, u32 long_refresh)
{
	const struct fb_videomode *best;

	best = viafb_get_best_mode(hres, vres, long_refresh);
	if (!best)
		return 60;

	if (abs(best->refresh - long_refresh) > 3) {
		if (hres == 1200 && vres == 900)
			return 49; /*                               */
		else
			return 60;
	}

	return best->refresh;
}

static void device_off(void)
{
	viafb_dvi_disable();
	viafb_lcd_disable();
}

static void device_on(void)
{
	if (viafb_DVI_ON == 1)
		viafb_dvi_enable();
	if (viafb_LCD_ON == 1)
		viafb_lcd_enable();
}

static void enable_second_display_channel(void)
{
	/*                                   */
	viafb_write_reg_mask(CR6A, VIACR, 0x00, BIT6);
	viafb_write_reg_mask(CR6A, VIACR, BIT7, BIT7);
	viafb_write_reg_mask(CR6A, VIACR, BIT6, BIT6);
}

static void disable_second_display_channel(void)
{
	/*                                    */
	viafb_write_reg_mask(CR6A, VIACR, 0x00, BIT6);
	viafb_write_reg_mask(CR6A, VIACR, 0x00, BIT7);
	viafb_write_reg_mask(CR6A, VIACR, BIT6, BIT6);
}

void viafb_set_dpa_gfx(int output_interface, struct GFX_DPA_SETTING\
					*p_gfx_dpa_setting)
{
	switch (output_interface) {
	case INTERFACE_DVP0:
		{
			/*                                 */
			viafb_write_reg_mask(CR96, VIACR,
				       p_gfx_dpa_setting->DVP0, 0x0F);

			/*                                   */
			viafb_write_reg_mask(SR1E, VIASR,
				       p_gfx_dpa_setting->DVP0ClockDri_S, BIT2);
			viafb_write_reg_mask(SR2A, VIASR,
				       p_gfx_dpa_setting->DVP0ClockDri_S1,
				       BIT4);
			viafb_write_reg_mask(SR1B, VIASR,
				       p_gfx_dpa_setting->DVP0DataDri_S, BIT1);
			viafb_write_reg_mask(SR2A, VIASR,
				       p_gfx_dpa_setting->DVP0DataDri_S1, BIT5);
			break;
		}

	case INTERFACE_DVP1:
		{
			/*                                 */
			viafb_write_reg_mask(CR9B, VIACR,
				       p_gfx_dpa_setting->DVP1, 0x0F);

			/*                                   */
			viafb_write_reg_mask(SR65, VIASR,
				       p_gfx_dpa_setting->DVP1Driving, 0x0F);
			break;
		}

	case INTERFACE_DFP_HIGH:
		{
			viafb_write_reg_mask(CR97, VIACR,
				       p_gfx_dpa_setting->DFPHigh, 0x0F);
			break;
		}

	case INTERFACE_DFP_LOW:
		{
			viafb_write_reg_mask(CR99, VIACR,
				       p_gfx_dpa_setting->DFPLow, 0x0F);
			break;
		}

	case INTERFACE_DFP:
		{
			viafb_write_reg_mask(CR97, VIACR,
				       p_gfx_dpa_setting->DFPHigh, 0x0F);
			viafb_write_reg_mask(CR99, VIACR,
				       p_gfx_dpa_setting->DFPLow, 0x0F);
			break;
		}
	}
}

void viafb_fill_var_timing_info(struct fb_var_screeninfo *var,
	const struct fb_videomode *mode)
{
	var->pixclock = mode->pixclock;
	var->xres = mode->xres;
	var->yres = mode->yres;
	var->left_margin = mode->left_margin;
	var->right_margin = mode->right_margin;
	var->hsync_len = mode->hsync_len;
	var->upper_margin = mode->upper_margin;
	var->lower_margin = mode->lower_margin;
	var->vsync_len = mode->vsync_len;
	var->sync = mode->sync;
}