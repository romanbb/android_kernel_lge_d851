/*
 * Quickcam cameras initialization data
 *
 * V4L2 by Jean-Francois Moine <http://moinejf.free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#define MODULE_NAME "tv8532"

#include "gspca.h"

MODULE_AUTHOR("Michel Xhaard <mxhaard@users.sourceforge.net>");
MODULE_DESCRIPTION("TV8532 USB Camera Driver");
MODULE_LICENSE("GPL");

/*                            */
struct sd {
	struct gspca_dev gspca_dev;	/*                           */

	__u16 exposure;
	__u16 gain;

	__u8 packet;
};

/*                                       */
static int sd_setexposure(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getexposure(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setgain(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getgain(struct gspca_dev *gspca_dev, __s32 *val);

static const struct ctrl sd_ctrls[] = {
	{
	 {
	  .id = V4L2_CID_EXPOSURE,
	  .type = V4L2_CTRL_TYPE_INTEGER,
	  .name = "Exposure",
	  .minimum = 1,
	  .maximum = 0x18f,
	  .step = 1,
#define EXPOSURE_DEF 0x18f
	  .default_value = EXPOSURE_DEF,
	  },
	 .set = sd_setexposure,
	 .get = sd_getexposure,
	 },
	{
	 {
	  .id = V4L2_CID_GAIN,
	  .type = V4L2_CTRL_TYPE_INTEGER,
	  .name = "Gain",
	  .minimum = 0,
	  .maximum = 0x7ff,
	  .step = 1,
#define GAIN_DEF 0x100
	  .default_value = GAIN_DEF,
	  },
	 .set = sd_setgain,
	 .get = sd_getgain,
	 },
};

static const struct v4l2_pix_format sif_mode[] = {
	{176, 144, V4L2_PIX_FMT_SBGGR8, V4L2_FIELD_NONE,
		.bytesperline = 176,
		.sizeimage = 176 * 144,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 1},
	{352, 288, V4L2_PIX_FMT_SBGGR8, V4L2_FIELD_NONE,
		.bytesperline = 352,
		.sizeimage = 352 * 288,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0},
};

/*                                   */
#define R00_PART_CONTROL 0x00
#define		LATENT_CHANGE	0x80
#define		EXPO_CHANGE	0x04
#define R01_TIMING_CONTROL_LOW 0x01
#define		CMD_EEprom_Open 0x30
#define		CMD_EEprom_Close 0x29
#define R03_TABLE_ADDR 0x03
#define R04_WTRAM_DATA_L 0x04
#define R05_WTRAM_DATA_M 0x05
#define R06_WTRAM_DATA_H 0x06
#define R07_TABLE_LEN	0x07
#define R08_RAM_WRITE_ACTION 0x08
#define R0C_AD_WIDTHL	0x0c
#define R0D_AD_WIDTHH	0x0d
#define R0E_AD_HEIGHTL	0x0e
#define R0F_AD_HEIGHTH	0x0f
#define R10_AD_COL_BEGINL 0x10
#define R11_AD_COL_BEGINH 0x11
#define		MIRROR		0x04	/*      */
#define R14_AD_ROW_BEGINL 0x14
#define R15_AD_ROWBEGINH  0x15
#define R1C_AD_EXPOSE_TIMEL 0x1c
#define R20_GAIN_G1L	0x20
#define R21_GAIN_G1H	0x21
#define R22_GAIN_RL	0x22
#define R23_GAIN_RH	0x23
#define R24_GAIN_BL	0x24
#define R25_GAIN_BH	0x25
#define R26_GAIN_G2L	0x26
#define R27_GAIN_G2H	0x27
#define R28_QUANT	0x28
#define R29_LINE	0x29
#define R2C_POLARITY	0x2c
#define R2D_POINT	0x2d
#define R2E_POINTH	0x2e
#define R2F_POINTB	0x2f
#define R30_POINTBH	0x30
#define R31_UPD		0x31
#define R2A_HIGH_BUDGET 0x2a
#define R2B_LOW_BUDGET	0x2b
#define R34_VID		0x34
#define R35_VIDH	0x35
#define R36_PID		0x36
#define R37_PIDH	0x37
#define R39_Test1	0x39		/*      */
#define R3B_Test3	0x3b		/*      */
#define R83_AD_IDH	0x83
#define R91_AD_SLOPEREG 0x91
#define R94_AD_BITCONTROL 0x94

static const u8 eeprom_data[][3] = {
/*                   */
	{0x01, 0x00, 0x01},
	{0x01, 0x80, 0x11},
	{0x05, 0x00, 0x14},
	{0x05, 0x00, 0x1c},
	{0x0d, 0x00, 0x1e},
	{0x05, 0x00, 0x1f},
	{0x05, 0x05, 0x19},
	{0x05, 0x01, 0x1b},
	{0x05, 0x09, 0x1e},
	{0x0d, 0x89, 0x2e},
	{0x05, 0x89, 0x2f},
	{0x05, 0x0d, 0xd9},
	{0x05, 0x09, 0xf1},
};


/*              */
static void reg_w1(struct gspca_dev *gspca_dev,
		  __u16 index, __u8 value)
{
	gspca_dev->usb_buf[0] = value;
	usb_control_msg(gspca_dev->dev,
			usb_sndctrlpipe(gspca_dev->dev, 0),
			0x02,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			0,	/*       */
			index, gspca_dev->usb_buf, 1, 500);
}

/*               */
static void reg_w2(struct gspca_dev *gspca_dev,
		  u16 index, u16 value)
{
	gspca_dev->usb_buf[0] = value;
	gspca_dev->usb_buf[1] = value >> 8;
	usb_control_msg(gspca_dev->dev,
			usb_sndctrlpipe(gspca_dev->dev, 0),
			0x02,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			0,	/*       */
			index, gspca_dev->usb_buf, 2, 500);
}

static void tv_8532WriteEEprom(struct gspca_dev *gspca_dev)
{
	int i;

	reg_w1(gspca_dev, R01_TIMING_CONTROL_LOW, CMD_EEprom_Open);
	for (i = 0; i < ARRAY_SIZE(eeprom_data); i++) {
		reg_w1(gspca_dev, R03_TABLE_ADDR, i);
		reg_w1(gspca_dev, R04_WTRAM_DATA_L, eeprom_data[i][2]);
		reg_w1(gspca_dev, R05_WTRAM_DATA_M, eeprom_data[i][1]);
		reg_w1(gspca_dev, R06_WTRAM_DATA_H, eeprom_data[i][0]);
		reg_w1(gspca_dev, R08_RAM_WRITE_ACTION, 0);
	}
	reg_w1(gspca_dev, R07_TABLE_LEN, i);
	reg_w1(gspca_dev, R01_TIMING_CONTROL_LOW, CMD_EEprom_Close);
}

/*                                       */
static int sd_config(struct gspca_dev *gspca_dev,
		     const struct usb_device_id *id)
{
	struct sd *sd = (struct sd *) gspca_dev;
	struct cam *cam;

	cam = &gspca_dev->cam;
	cam->cam_mode = sif_mode;
	cam->nmodes = ARRAY_SIZE(sif_mode);

	sd->exposure = EXPOSURE_DEF;
	sd->gain = GAIN_DEF;
	return 0;
}

static void tv_8532_setReg(struct gspca_dev *gspca_dev)
{
	reg_w1(gspca_dev, R3B_Test3, 0x0a);	/*               */
	/*                                                    */
	reg_w1(gspca_dev, R0E_AD_HEIGHTL, 0x90);
	reg_w1(gspca_dev, R0F_AD_HEIGHTH, 0x01);
	reg_w2(gspca_dev, R1C_AD_EXPOSE_TIMEL, 0x018f);
	reg_w1(gspca_dev, R10_AD_COL_BEGINL, 0x44);
						/*                   */
	reg_w1(gspca_dev, R11_AD_COL_BEGINH, 0x00);
						/*                         */
	reg_w1(gspca_dev, R14_AD_ROW_BEGINL, 0x0a);

	reg_w1(gspca_dev, R94_AD_BITCONTROL, 0x02);
	reg_w1(gspca_dev, R91_AD_SLOPEREG, 0x00);
	reg_w1(gspca_dev, R00_PART_CONTROL, LATENT_CHANGE | EXPO_CHANGE);
						/*        */
}

/*                                                  */
static int sd_init(struct gspca_dev *gspca_dev)
{
	tv_8532WriteEEprom(gspca_dev);

	return 0;
}

static void setexposure(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	reg_w2(gspca_dev, R1C_AD_EXPOSE_TIMEL, sd->exposure);
	reg_w1(gspca_dev, R00_PART_CONTROL, LATENT_CHANGE | EXPO_CHANGE);
						/*      */
}

static void setgain(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	reg_w2(gspca_dev, R20_GAIN_G1L, sd->gain);
	reg_w2(gspca_dev, R22_GAIN_RL, sd->gain);
	reg_w2(gspca_dev, R24_GAIN_BL, sd->gain);
	reg_w2(gspca_dev, R26_GAIN_G2L, sd->gain);
}

/*                        */
static int sd_start(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	reg_w1(gspca_dev, R0C_AD_WIDTHL, 0xe8);		/*            */
	reg_w1(gspca_dev, R0D_AD_WIDTHH, 0x03);

	/*                                              */
	reg_w1(gspca_dev, R28_QUANT, 0x90);
					/*                           */
	if (gspca_dev->cam.cam_mode[(int) gspca_dev->curr_mode].priv) {
		/*         */
		reg_w1(gspca_dev, R29_LINE, 0x41);
					/*                      */
	} else {
		/*         */
		reg_w1(gspca_dev, R29_LINE, 0x81);
					/*                      */
	}
	/*                                              */
	reg_w1(gspca_dev, R2C_POLARITY, 0x10);		/*            */
	reg_w1(gspca_dev, R2D_POINT, 0x14);
	reg_w1(gspca_dev, R2E_POINTH, 0x01);
	reg_w1(gspca_dev, R2F_POINTB, 0x12);
	reg_w1(gspca_dev, R30_POINTBH, 0x01);

	tv_8532_setReg(gspca_dev);

	setexposure(gspca_dev);
	setgain(gspca_dev);

	/*                                              */
	reg_w1(gspca_dev, R31_UPD, 0x01);	/*                  */
	msleep(200);
	reg_w1(gspca_dev, R31_UPD, 0x00);	/*            */

	gspca_dev->empty_packet = 0;		/*                         */
	sd->packet = 0;				/*                          */

	return 0;
}

static void sd_stopN(struct gspca_dev *gspca_dev)
{
	reg_w1(gspca_dev, R3B_Test3, 0x0b);	/*                      */
}

static void sd_pkt_scan(struct gspca_dev *gspca_dev,
			u8 *data,			/*             */
			int len)			/*                   */
{
	struct sd *sd = (struct sd *) gspca_dev;
	int packet_type0, packet_type1;

	packet_type0 = packet_type1 = INTER_PACKET;
	if (gspca_dev->empty_packet) {
		gspca_dev->empty_packet = 0;
		sd->packet = gspca_dev->height / 2;
		packet_type0 = FIRST_PACKET;
	} else if (sd->packet == 0)
		return;			/*                           */
	sd->packet--;
	if (sd->packet == 0)
		packet_type1 = LAST_PACKET;

	/*                      
                    
               
             
               
             
  */
	gspca_frame_add(gspca_dev, packet_type0,
			data + 2, gspca_dev->width);
	gspca_frame_add(gspca_dev, packet_type1,
			data + gspca_dev->width + 5, gspca_dev->width);
}

static int sd_setexposure(struct gspca_dev *gspca_dev, __s32 val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->exposure = val;
	if (gspca_dev->streaming)
		setexposure(gspca_dev);
	return 0;
}

static int sd_getexposure(struct gspca_dev *gspca_dev, __s32 *val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	*val = sd->exposure;
	return 0;
}

static int sd_setgain(struct gspca_dev *gspca_dev, __s32 val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->gain = val;
	if (gspca_dev->streaming)
		setgain(gspca_dev);
	return 0;
}

static int sd_getgain(struct gspca_dev *gspca_dev, __s32 *val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	*val = sd->gain;
	return 0;
}

/*                        */
static const struct sd_desc sd_desc = {
	.name = MODULE_NAME,
	.ctrls = sd_ctrls,
	.nctrls = ARRAY_SIZE(sd_ctrls),
	.config = sd_config,
	.init = sd_init,
	.start = sd_start,
	.stopN = sd_stopN,
	.pkt_scan = sd_pkt_scan,
};

/*                             */
static const struct usb_device_id device_table[] = {
	{USB_DEVICE(0x046d, 0x0920)},
	{USB_DEVICE(0x046d, 0x0921)},
	{USB_DEVICE(0x0545, 0x808b)},
	{USB_DEVICE(0x0545, 0x8333)},
	{USB_DEVICE(0x0923, 0x010f)},
	{}
};

MODULE_DEVICE_TABLE(usb, device_table);

/*                      */
static int sd_probe(struct usb_interface *intf,
		    const struct usb_device_id *id)
{
	return gspca_dev_probe(intf, id, &sd_desc, sizeof(struct sd),
			       THIS_MODULE);
}

static struct usb_driver sd_driver = {
	.name = MODULE_NAME,
	.id_table = device_table,
	.probe = sd_probe,
	.disconnect = gspca_disconnect,
#ifdef CONFIG_PM
	.suspend = gspca_suspend,
	.resume = gspca_resume,
#endif
};

module_usb_driver(sd_driver);