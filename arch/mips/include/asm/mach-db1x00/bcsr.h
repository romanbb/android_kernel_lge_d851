/*
                                                                        
  
                                                                       
                                                                       
                                                                       
             
  
                                                                   
 */

#ifndef _DB1XXX_BCSR_H_
#define _DB1XXX_BCSR_H_


/*                                                                 
                                                                   
                                              
 */

/*                                        */
#define DB1000_BCSR_PHYS_ADDR	0x0E000000
#define DB1000_BCSR_HEXLED_OFS	0x01000000

#define DB1550_BCSR_PHYS_ADDR	0x0F000000
#define DB1550_BCSR_HEXLED_OFS	0x00400000

#define PB1550_BCSR_PHYS_ADDR	0x0F000000
#define PB1550_BCSR_HEXLED_OFS	0x00800000

#define DB1200_BCSR_PHYS_ADDR	0x19800000
#define DB1200_BCSR_HEXLED_OFS	0x00400000

#define PB1200_BCSR_PHYS_ADDR	0x0D800000
#define PB1200_BCSR_HEXLED_OFS	0x00400000

#define DB1300_BCSR_PHYS_ADDR	0x19800000
#define DB1300_BCSR_HEXLED_OFS	0x00400000

enum bcsr_id {
	/*             */
	BCSR_WHOAMI	= 0,
	BCSR_STATUS,
	BCSR_SWITCHES,
	BCSR_RESETS,
	BCSR_PCMCIA,
	BCSR_BOARD,
	BCSR_LEDS,
	BCSR_SYSTEM,
	/*                          */
	BCSR_INTCLR,
	BCSR_INTSET,
	BCSR_MASKCLR,
	BCSR_MASKSET,
	BCSR_SIGSTAT,
	BCSR_INTSTAT,

	/*             */
	BCSR_HEXLEDS,
	BCSR_RSVD1,
	BCSR_HEXCLEAR,

	BCSR_CNT,
};

/*                                                      */
#define BCSR_REG_WHOAMI		0x00
#define BCSR_REG_STATUS		0x04
#define BCSR_REG_SWITCHES	0x08
#define BCSR_REG_RESETS		0x0c
#define BCSR_REG_PCMCIA		0x10
#define BCSR_REG_BOARD		0x14
#define BCSR_REG_LEDS		0x18
#define BCSR_REG_SYSTEM		0x1c
/*                                            */
#define BCSR_REG_INTCLR		0x20
#define BCSR_REG_INTSET		0x24
#define BCSR_REG_MASKCLR	0x28
#define BCSR_REG_MASKSET	0x2c
#define BCSR_REG_SIGSTAT	0x30
#define BCSR_REG_INTSTAT	0x34

/*                                         */
#define BCSR_REG_HEXLEDS	0x00
#define BCSR_REG_HEXCLEAR	0x08

/*
                            
 */
#define BCSR_WHOAMI_DCID(x)		((x) & 0xf)
#define BCSR_WHOAMI_CPLD(x)		(((x) >> 4) & 0xf)
#define BCSR_WHOAMI_BOARD(x)		(((x) >> 8) & 0xf)

/*                                                */
enum bcsr_whoami_boards {
	BCSR_WHOAMI_PB1500 = 1,
	BCSR_WHOAMI_PB1500R2,
	BCSR_WHOAMI_PB1100,
	BCSR_WHOAMI_DB1000,
	BCSR_WHOAMI_DB1100,
	BCSR_WHOAMI_DB1500,
	BCSR_WHOAMI_DB1550,
	BCSR_WHOAMI_PB1550_DDR,
	BCSR_WHOAMI_PB1550 = BCSR_WHOAMI_PB1550_DDR,
	BCSR_WHOAMI_PB1550_SDR,
	BCSR_WHOAMI_PB1200_DDR1,
	BCSR_WHOAMI_PB1200 = BCSR_WHOAMI_PB1200_DDR1,
	BCSR_WHOAMI_PB1200_DDR2,
	BCSR_WHOAMI_DB1200,
	BCSR_WHOAMI_DB1300,
};

/*                                                                  
                   
 */
#define BCSR_STATUS_PC0VS		0x0003
#define BCSR_STATUS_PC1VS		0x000C
#define BCSR_STATUS_PC0FI		0x0010
#define BCSR_STATUS_PC1FI		0x0020
#define BCSR_STATUS_PB1550_SWAPBOOT	0x0040
#define BCSR_STATUS_SRAMWIDTH		0x0080
#define BCSR_STATUS_FLASHBUSY		0x0100
#define BCSR_STATUS_ROMBUSY		0x0400
#define BCSR_STATUS_SD0WP		0x0400	/*                   */
#define BCSR_STATUS_SD1WP		0x0800
#define BCSR_STATUS_USBOTGID		0x0800	/*           */
#define BCSR_STATUS_DB1000_SWAPBOOT	0x2000
#define BCSR_STATUS_DB1200_SWAPBOOT	0x0040	/*             */
#define BCSR_STATUS_IDECBLID		0x0200	/*             */
#define BCSR_STATUS_DB1200_U0RXD	0x1000	/*        */
#define BCSR_STATUS_DB1200_U1RXD	0x2000	/*        */
#define BCSR_STATUS_FLASHDEN		0xC000
#define BCSR_STATUS_DB1550_U0RXD	0x1000	/*        */
#define BCSR_STATUS_DB1550_U3RXD	0x2000	/*        */
#define BCSR_STATUS_PB1550_U0RXD	0x1000	/*        */
#define BCSR_STATUS_PB1550_U1RXD	0x2000	/*        */
#define BCSR_STATUS_PB1550_U3RXD	0x8000	/*        */

#define BCSR_STATUS_CFWP		0x4000	/*        */
#define BCSR_STATUS_USBOCn		0x2000	/*        */
#define BCSR_STATUS_OTGOCn		0x1000	/*        */
#define BCSR_STATUS_DCDMARQ		0x0010	/*        */
#define BCSR_STATUS_IDEDMARQ		0x0020	/*        */

/*                          */
#define BCSR_RESETS_PHY0		0x0001
#define BCSR_RESETS_PHY1		0x0002
#define BCSR_RESETS_DC			0x0004
#define BCSR_RESETS_FIR_SEL		0x2000
#define BCSR_RESETS_IRDA_MODE_MASK	0xC000
#define BCSR_RESETS_IRDA_MODE_FULL	0x0000
#define BCSR_RESETS_PB1550_WSCFSM	0x2000
#define BCSR_RESETS_IRDA_MODE_OFF	0x4000
#define BCSR_RESETS_IRDA_MODE_2_3	0x8000
#define BCSR_RESETS_IRDA_MODE_1_3	0xC000
#define BCSR_RESETS_DMAREQ		0x8000	/*        */

#define BCSR_BOARD_PCIM66EN		0x0001
#define BCSR_BOARD_SD0PWR		0x0040
#define BCSR_BOARD_SD1PWR		0x0080
#define BCSR_BOARD_PCIM33		0x0100
#define BCSR_BOARD_PCIEXTARB		0x0200
#define BCSR_BOARD_GPIO200RST		0x0400
#define BCSR_BOARD_PCICLKOUT		0x0800
#define BCSR_BOARD_PCICFG		0x1000
#define BCSR_BOARD_SPISEL		0x2000	/*           */
#define BCSR_BOARD_SD0WP		0x4000	/*        */
#define BCSR_BOARD_SD1WP		0x8000	/*        */


/*                */
#define BCSR_RESETS_ETH			0x0001
#define BCSR_RESETS_CAMERA		0x0002
#define BCSR_RESETS_DC			0x0004
#define BCSR_RESETS_IDE			0x0008
#define BCSR_RESETS_TV			0x0010	/*             */
/*                                     */
#define BCSR_RESETS_PWMR1MUX		0x0800	/*        */
#define BCSR_RESETS_PB1200_WSCFSM	0x0800	/*        */
#define BCSR_RESETS_PSC0MUX		0x1000
#define BCSR_RESETS_PSC1MUX		0x2000
#define BCSR_RESETS_SPISEL		0x4000
#define BCSR_RESETS_SD1MUX		0x8000	/*        */

#define BCSR_RESETS_VDDQSHDN		0x0200	/*        */
#define BCSR_RESETS_OTPPGM		0x0400	/*        */
#define BCSR_RESETS_OTPSCLK		0x0800	/*        */
#define BCSR_RESETS_OTPWRPROT		0x1000	/*        */
#define BCSR_RESETS_OTPCSB		0x2000	/*        */
#define BCSR_RESETS_OTGPWR		0x4000	/*        */
#define BCSR_RESETS_USBHPWR		0x8000  /*        */

#define BCSR_BOARD_LCDVEE		0x0001
#define BCSR_BOARD_LCDVDD		0x0002
#define BCSR_BOARD_LCDBL		0x0004
#define BCSR_BOARD_CAMSNAP		0x0010
#define BCSR_BOARD_CAMPWR		0x0020
#define BCSR_BOARD_SD0PWR		0x0040
#define BCSR_BOARD_CAMCS		0x0010	/*        */
#define BCSR_BOARD_HDMI_DE		0x0040	/*        */

#define BCSR_SWITCHES_DIP		0x00FF
#define BCSR_SWITCHES_DIP_1		0x0080
#define BCSR_SWITCHES_DIP_2		0x0040
#define BCSR_SWITCHES_DIP_3		0x0020
#define BCSR_SWITCHES_DIP_4		0x0010
#define BCSR_SWITCHES_DIP_5		0x0008
#define BCSR_SWITCHES_DIP_6		0x0004
#define BCSR_SWITCHES_DIP_7		0x0002
#define BCSR_SWITCHES_DIP_8		0x0001
#define BCSR_SWITCHES_ROTARY		0x0F00


#define BCSR_PCMCIA_PC0VPP		0x0003
#define BCSR_PCMCIA_PC0VCC		0x000C
#define BCSR_PCMCIA_PC0DRVEN		0x0010
#define BCSR_PCMCIA_PC0RST		0x0080
#define BCSR_PCMCIA_PC1VPP		0x0300
#define BCSR_PCMCIA_PC1VCC		0x0C00
#define BCSR_PCMCIA_PC1DRVEN		0x1000
#define BCSR_PCMCIA_PC1RST		0x8000


#define BCSR_LEDS_DECIMALS		0x0003
#define BCSR_LEDS_LED0			0x0100
#define BCSR_LEDS_LED1			0x0200
#define BCSR_LEDS_LED2			0x0400
#define BCSR_LEDS_LED3			0x0800


#define BCSR_SYSTEM_RESET		0x8000	/*                */
#define BCSR_SYSTEM_PWROFF		0x4000	/*                  */
#define BCSR_SYSTEM_VDDI		0x001F	/*               */
#define BCSR_SYSTEM_DEBUGCSMASK		0x003F	/*        */
#define BCSR_SYSTEM_UDMAMODE		0x0100	/*        */
#define BCSR_SYSTEM_WAKEONIRQ		0x0200	/*        */
#define BCSR_SYSTEM_VDDI1300		0x3C00	/*        */



/*                                                                    
               
 */
void __init bcsr_init(unsigned long bcsr1_phys, unsigned long bcsr2_phys);

/*                       */
unsigned short bcsr_read(enum bcsr_id reg);

/*                           */
void bcsr_write(enum bcsr_id reg, unsigned short val);

/*                                                                   */
void bcsr_mod(enum bcsr_id reg, unsigned short clr, unsigned short set);

/*                                          */
void __init bcsr_init_irq(int csc_start, int csc_end, int hook_irq);

#endif
