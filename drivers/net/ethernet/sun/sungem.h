/* $Id: sungem.h,v 1.10.2.4 2002/03/11 08:54:48 davem Exp $
 * sungem.h: Definitions for Sun GEM ethernet driver.
 *
 * Copyright (C) 2000 David S. Miller (davem@redhat.com)
 */

#ifndef _SUNGEM_H
#define _SUNGEM_H

/*                  */
#define GREG_SEBSTATE	0x0000UL	/*                     */
#define GREG_CFG	0x0004UL	/*                        */
#define GREG_STAT	0x000CUL	/*                  */
#define GREG_IMASK	0x0010UL	/*                         */
#define GREG_IACK	0x0014UL	/*                        */
#define GREG_STAT2	0x001CUL	/*                     */
#define GREG_PCIESTAT	0x1000UL	/*                           */
#define GREG_PCIEMASK	0x1004UL	/*                         */
#define GREG_BIFCFG	0x1008UL	/*                            */
#define GREG_BIFDIAG	0x100CUL	/*                          */
#define GREG_SWRST	0x1010UL	/*                         */

/*                           */
#define GREG_SEBSTATE_ARB	0x00000003	/*                   */
#define GREG_SEBSTATE_RXWON	0x00000004	/*                             */

/*                               */
#define GREG_CFG_IBURST		0x00000001	/*                 */
#define GREG_CFG_TXDMALIM	0x0000003e	/*                     */
#define GREG_CFG_RXDMALIM	0x000007c0	/*                     */
#define GREG_CFG_RONPAULBIT	0x00000800	/*                                   
                                      */
#define GREG_CFG_ENBUG2FIX	0x00001000	/*                            */

/*                                  
  
                                                               
                                                                 
                                                                   
                                                                
                                                                  
                      
 */
#define GREG_STAT_TXINTME	0x00000001	/*                            */
#define GREG_STAT_TXALL		0x00000002	/*                           */
#define GREG_STAT_TXDONE	0x00000004	/*                          */
#define GREG_STAT_RXDONE	0x00000010	/*                       */
#define GREG_STAT_RXNOBUF	0x00000020	/*                              */
#define GREG_STAT_RXTAGERR	0x00000040	/*                           */
#define GREG_STAT_PCS		0x00002000	/*                         */
#define GREG_STAT_TXMAC		0x00004000	/*                            */
#define GREG_STAT_RXMAC		0x00008000	/*                            */
#define GREG_STAT_MAC		0x00010000	/*                           */
#define GREG_STAT_MIF		0x00020000	/*                         */
#define GREG_STAT_PCIERR	0x00040000	/*                      */
#define GREG_STAT_TXNR		0xfff80000	/*                         */
#define GREG_STAT_TXNR_SHIFT	19

#define GREG_STAT_ABNORMAL	(GREG_STAT_RXNOBUF | GREG_STAT_RXTAGERR | \
				 GREG_STAT_PCS | GREG_STAT_TXMAC | GREG_STAT_RXMAC | \
				 GREG_STAT_MAC | GREG_STAT_MIF | GREG_STAT_PCIERR)

#define GREG_STAT_NAPI		(GREG_STAT_TXALL  | GREG_STAT_TXINTME | \
				 GREG_STAT_RXDONE | GREG_STAT_ABNORMAL)

/*                                                                  
                                                                     
                                                                           
                                                                             
                                                                            
                
 */

/*                                  */
#define GREG_PCIESTAT_BADACK	0x00000001	/*                              */
#define GREG_PCIESTAT_DTRTO	0x00000002	/*                             */
#define GREG_PCIESTAT_OTHER	0x00000004	/*                                  */

/*                                                                       
                                                                        
                        
 */

/*                                   */
#define GREG_BIFCFG_SLOWCLK	0x00000001	/*                         */
#define GREG_BIFCFG_B64DIS	0x00000002	/*                              */
#define GREG_BIFCFG_M66EN	0x00000004	/*                             */

/*                                 */
#define GREG_BIFDIAG_BURSTSM	0x007f0000	/*                         */
#define GREG_BIFDIAG_BIFSM	0xff000000	/*                    */

/*                                
  
                                                                            
                                                                         
                                                                           
                                                         
 */
#define GREG_SWRST_TXRST	0x00000001	/*                    */
#define GREG_SWRST_RXRST	0x00000002	/*                    */
#define GREG_SWRST_RSTOUT	0x00000004	/*                       */
#define GREG_SWRST_CACHESIZE	0x00ff0000	/*                           */
#define GREG_SWRST_CACHE_SHIFT	16

/*                  */
#define TXDMA_KICK	0x2000UL	/*                   */
#define TXDMA_CFG	0x2004UL	/*                           */
#define TXDMA_DBLOW	0x2008UL	/*                    */
#define TXDMA_DBHI	0x200CUL	/*                     */
#define TXDMA_FWPTR	0x2014UL	/*                       */
#define TXDMA_FSWPTR	0x2018UL	/*                              */
#define TXDMA_FRPTR	0x201CUL	/*                       */
#define TXDMA_FSRPTR	0x2020UL	/*                             */
#define TXDMA_PCNT	0x2024UL	/*                        */
#define TXDMA_SMACHINE	0x2028UL	/*                           */
#define TXDMA_DPLOW	0x2030UL	/*                      */
#define TXDMA_DPHI	0x2034UL	/*                       */
#define TXDMA_TXDONE	0x2100UL	/*                        */
#define TXDMA_FADDR	0x2104UL	/*                  */
#define TXDMA_FTAG	0x2108UL	/*               */
#define TXDMA_DLOW	0x210CUL	/*                   */
#define TXDMA_DHIT1	0x2110UL	/*                      */
#define TXDMA_DHIT0	0x2114UL	/*                      */
#define TXDMA_FSZ	0x2118UL	/*                */

/*                  
  
                                                                          
                                                                
 */

/*                        
  
                                                                           
                                                                         
                                                                        
                                                                  
 */

/*                           
  
                                                                             
                                                                         
                                                                         
 */
#define TXDMA_CFG_ENABLE	0x00000001	/*                       */
#define TXDMA_CFG_RINGSZ	0x0000001e	/*                         */
#define TXDMA_CFG_RINGSZ_32	0x00000000	/*                    */
#define TXDMA_CFG_RINGSZ_64	0x00000002	/*                    */
#define TXDMA_CFG_RINGSZ_128	0x00000004	/*                     */
#define TXDMA_CFG_RINGSZ_256	0x00000006	/*                     */
#define TXDMA_CFG_RINGSZ_512	0x00000008	/*                     */
#define TXDMA_CFG_RINGSZ_1K	0x0000000a	/*                      */
#define TXDMA_CFG_RINGSZ_2K	0x0000000c	/*                      */
#define TXDMA_CFG_RINGSZ_4K	0x0000000e	/*                      */
#define TXDMA_CFG_RINGSZ_8K	0x00000010	/*                      */
#define TXDMA_CFG_PIOSEL	0x00000020	/*                             */
#define TXDMA_CFG_FTHRESH	0x001ffc00	/*                             */
#define TXDMA_CFG_PMODE		0x00200000	/*                              */

/*                             
  
                                                                             
                                                                        
                                                                  
 */

/*                                                                                 
                     
 */

/*                     */
#define WOL_MATCH0	0x3000UL
#define WOL_MATCH1	0x3004UL
#define WOL_MATCH2	0x3008UL
#define WOL_MCOUNT	0x300CUL
#define WOL_WAKECSR	0x3010UL

/*                         
 */
#define WOL_MCOUNT_N		0x00000010
#define WOL_MCOUNT_M		0x00000000 /*        */

#define WOL_WAKECSR_ENABLE	0x00000001
#define WOL_WAKECSR_MII		0x00000002
#define WOL_WAKECSR_SEEN	0x00000004
#define WOL_WAKECSR_FILT_UCAST	0x00000008
#define WOL_WAKECSR_FILT_MCAST	0x00000010
#define WOL_WAKECSR_FILT_BCAST	0x00000020
#define WOL_WAKECSR_FILT_SEEN	0x00000040


/*                       */
#define RXDMA_CFG	0x4000UL	/*                           */
#define RXDMA_DBLOW	0x4004UL	/*                        */
#define RXDMA_DBHI	0x4008UL	/*                         */
#define RXDMA_FWPTR	0x400CUL	/*                       */
#define RXDMA_FSWPTR	0x4010UL	/*                              */
#define RXDMA_FRPTR	0x4014UL	/*                       */
#define RXDMA_PCNT	0x4018UL	/*                        */
#define RXDMA_SMACHINE	0x401CUL	/*                           */
#define RXDMA_PTHRESH	0x4020UL	/*                   */
#define RXDMA_DPLOW	0x4024UL	/*                      */
#define RXDMA_DPHI	0x4028UL	/*                       */
#define RXDMA_KICK	0x4100UL	/*                   */
#define RXDMA_DONE	0x4104UL	/*                        */
#define RXDMA_BLANK	0x4108UL	/*                       */
#define RXDMA_FADDR	0x410CUL	/*                  */
#define RXDMA_FTAG	0x4110UL	/*               */
#define RXDMA_DLOW	0x4114UL	/*                   */
#define RXDMA_DHIT1	0x4118UL	/*                      */
#define RXDMA_DHIT0	0x411CUL	/*                      */
#define RXDMA_FSZ	0x4120UL	/*                */

/*                            */
#define RXDMA_CFG_ENABLE	0x00000001	/*                       */
#define RXDMA_CFG_RINGSZ	0x0000001e	/*                         */
#define RXDMA_CFG_RINGSZ_32	0x00000000	/*                 */
#define RXDMA_CFG_RINGSZ_64	0x00000002	/*                 */
#define RXDMA_CFG_RINGSZ_128	0x00000004	/*                 */
#define RXDMA_CFG_RINGSZ_256	0x00000006	/*                 */
#define RXDMA_CFG_RINGSZ_512	0x00000008	/*                 */
#define RXDMA_CFG_RINGSZ_1K	0x0000000a	/*                 */
#define RXDMA_CFG_RINGSZ_2K	0x0000000c	/*                 */
#define RXDMA_CFG_RINGSZ_4K	0x0000000e	/*                 */
#define RXDMA_CFG_RINGSZ_8K	0x00000010	/*                 */
#define RXDMA_CFG_RINGSZ_BDISAB	0x00000020	/*                          */
#define RXDMA_CFG_FBOFF		0x00001c00	/*                           */
#define RXDMA_CFG_CSUMOFF	0x000fe000	/*                             */
#define RXDMA_CFG_FTHRESH	0x07000000	/*                             */
#define RXDMA_CFG_FTHRESH_64	0x00000000	/*                */
#define RXDMA_CFG_FTHRESH_128	0x01000000	/*                */
#define RXDMA_CFG_FTHRESH_256	0x02000000	/*                */
#define RXDMA_CFG_FTHRESH_512	0x03000000	/*                */
#define RXDMA_CFG_FTHRESH_1K	0x04000000	/*                */
#define RXDMA_CFG_FTHRESH_2K	0x05000000	/*                */

/*                             
  
                                                                             
                                                                        
                                                                  
 */

/*                     
  
                                                                       
                                                                       
 */
#define RXDMA_PTHRESH_OFF	0x000001ff	/*                            */
#define RXDMA_PTHRESH_ON	0x001ff000	/*                           */

/*                  
  
                                                                     
                                                                      
                                                                      
                 
  
                                                                     
                                                                  
                                                                     
                      
 */

/*                        
  
                                                                          
                                                                        
                                                                       
                                                                        
                                                                       
                             
 */

/*                       */
#define RXDMA_BLANK_IPKTS	0x000001ff	/*                            
                                    
                          
       */
#define RXDMA_BLANK_ITIME	0x000ff000	/*                            
                                      
                                       
                              
       */

/*              
  
                                                                            
                                                                           
              
 */

/*                                                                                 
                     
 */

/*               */
#define MAC_TXRST	0x6000UL	/*                              */
#define MAC_RXRST	0x6004UL	/*                              */
#define MAC_SNDPAUSE	0x6008UL	/*                             */
#define MAC_TXSTAT	0x6010UL	/*                        */
#define MAC_RXSTAT	0x6014UL	/*                        */
#define MAC_CSTAT	0x6018UL	/*                             */
#define MAC_TXMASK	0x6020UL	/*                       */
#define MAC_RXMASK	0x6024UL	/*                       */
#define MAC_MCMASK	0x6028UL	/*                           */
#define MAC_TXCFG	0x6030UL	/*                              */
#define MAC_RXCFG	0x6034UL	/*                              */
#define MAC_MCCFG	0x6038UL	/*                             */
#define MAC_XIFCFG	0x603CUL	/*                            */
#define MAC_IPG0	0x6040UL	/*                          */
#define MAC_IPG1	0x6044UL	/*                          */
#define MAC_IPG2	0x6048UL	/*                          */
#define MAC_STIME	0x604CUL	/*                    */
#define MAC_MINFSZ	0x6050UL	/*                       */
#define MAC_MAXFSZ	0x6054UL	/*                       */
#define MAC_PASIZE	0x6058UL	/*                   */
#define MAC_JAMSIZE	0x605CUL	/*                   */
#define MAC_ATTLIM	0x6060UL	/*                        */
#define MAC_MCTYPE	0x6064UL	/*                           */
#define MAC_ADDR0	0x6080UL	/*                        */
#define MAC_ADDR1	0x6084UL	/*                        */
#define MAC_ADDR2	0x6088UL	/*                        */
#define MAC_ADDR3	0x608CUL	/*                        */
#define MAC_ADDR4	0x6090UL	/*                        */
#define MAC_ADDR5	0x6094UL	/*                        */
#define MAC_ADDR6	0x6098UL	/*                        */
#define MAC_ADDR7	0x609CUL	/*                        */
#define MAC_ADDR8	0x60A0UL	/*                        */
#define MAC_AFILT0	0x60A4UL	/*                           */
#define MAC_AFILT1	0x60A8UL	/*                           */
#define MAC_AFILT2	0x60ACUL	/*                           */
#define MAC_AF21MSK	0x60B0UL	/*                             */
#define MAC_AF0MSK	0x60B4UL	/*                           */
#define MAC_HASH0	0x60C0UL	/*                       */
#define MAC_HASH1	0x60C4UL	/*                       */
#define MAC_HASH2	0x60C8UL	/*                       */
#define MAC_HASH3	0x60CCUL	/*                       */
#define MAC_HASH4	0x60D0UL	/*                       */
#define MAC_HASH5	0x60D4UL	/*                       */
#define MAC_HASH6	0x60D8UL	/*                       */
#define MAC_HASH7	0x60DCUL	/*                       */
#define MAC_HASH8	0x60E0UL	/*                       */
#define MAC_HASH9	0x60E4UL	/*                       */
#define MAC_HASH10	0x60E8UL	/*                        */
#define MAC_HASH11	0x60ECUL	/*                        */
#define MAC_HASH12	0x60F0UL	/*                        */
#define MAC_HASH13	0x60F4UL	/*                        */
#define MAC_HASH14	0x60F8UL	/*                        */
#define MAC_HASH15	0x60FCUL	/*                        */
#define MAC_NCOLL	0x6100UL	/*                          */
#define MAC_FASUCC	0x6104UL	/*                              */
#define MAC_ECOLL	0x6108UL	/*                             */
#define MAC_LCOLL	0x610CUL	/*                        */
#define MAC_DTIMER	0x6110UL	/*               */
#define MAC_PATMPS	0x6114UL	/*                        */
#define MAC_RFCTR	0x6118UL	/*                       */
#define MAC_LERR	0x611CUL	/*                       */
#define MAC_AERR	0x6120UL	/*                         */
#define MAC_FCSERR	0x6124UL	/*                    */
#define MAC_RXCVERR	0x6128UL	/*                             */
#define MAC_RANDSEED	0x6130UL	/*                             */
#define MAC_SMACHINE	0x6134UL	/*                        */

/*                                */
#define MAC_TXRST_CMD	0x00000001	/*                             */

/*                                */
#define MAC_RXRST_CMD	0x00000001	/*                             */

/*                     */
#define MAC_SNDPAUSE_TS	0x0000ffff	/*                               
                                   
                   
      */
#define MAC_SNDPAUSE_SP	0x00010000	/*                                   
                                    
                               
      */

/*                         */
#define MAC_TXSTAT_XMIT	0x00000001	/*                    */
#define MAC_TXSTAT_URUN	0x00000002	/*               */
#define MAC_TXSTAT_MPE	0x00000004	/*                       */
#define MAC_TXSTAT_NCE	0x00000008	/*                              */
#define MAC_TXSTAT_ECE	0x00000010	/*                              */
#define MAC_TXSTAT_LCE	0x00000020	/*                            */
#define MAC_TXSTAT_FCE	0x00000040	/*                             */
#define MAC_TXSTAT_DTE	0x00000080	/*                     */
#define MAC_TXSTAT_PCE	0x00000100	/*                           */

/*                         */
#define MAC_RXSTAT_RCV	0x00000001	/*                 */
#define MAC_RXSTAT_OFLW	0x00000002	/*                   */
#define MAC_RXSTAT_FCE	0x00000004	/*                    */
#define MAC_RXSTAT_ACE	0x00000008	/*                         */
#define MAC_RXSTAT_CCE	0x00000010	/*                       */
#define MAC_RXSTAT_LCE	0x00000020	/*                          */
#define MAC_RXSTAT_VCE	0x00000040	/*                            */

/*                              */
#define MAC_CSTAT_PRCV	0x00000001	/*                 */
#define MAC_CSTAT_PS	0x00000002	/*                */
#define MAC_CSTAT_NPS	0x00000004	/*                   */
#define MAC_CSTAT_PTR	0xffff0000	/*                      */

/*                                                                   
                                                                    
                                                                     
                                                                    
                                                                    
 */

/*                               
  
                                                               
                                                            
  
                                                          
                                                       
                                                         
                                                           
                        
 */
#define MAC_TXCFG_ENAB	0x00000001	/*                */
#define MAC_TXCFG_ICS	0x00000002	/*                       */
#define MAC_TXCFG_ICOLL	0x00000004	/*                    */
#define MAC_TXCFG_EIPG0	0x00000008	/*               */
#define MAC_TXCFG_NGU	0x00000010	/*                */
#define MAC_TXCFG_NGUL	0x00000020	/*                      */
#define MAC_TXCFG_NBO	0x00000040	/*              */
#define MAC_TXCFG_SD	0x00000080	/*             */
#define MAC_TXCFG_NFCS	0x00000100	/*          */
#define MAC_TXCFG_TCE	0x00000200	/*                       */

/*                               
  
                                                               
                                                            
  
                                                          
                                                                
                                                             
 */
#define MAC_RXCFG_ENAB	0x00000001	/*                */
#define MAC_RXCFG_SPAD	0x00000002	/*             */
#define MAC_RXCFG_SFCS	0x00000004	/*             */
#define MAC_RXCFG_PROM	0x00000008	/*                   */
#define MAC_RXCFG_PGRP	0x00000010	/*                    */
#define MAC_RXCFG_HFE	0x00000020	/*                     */
#define MAC_RXCFG_AFE	0x00000040	/*                       */
#define MAC_RXCFG_DDE	0x00000080	/*                          */
#define MAC_RXCFG_RCE	0x00000100	/*                       */

/*                              */
#define MAC_MCCFG_SPE	0x00000001	/*                    */
#define MAC_MCCFG_RPE	0x00000002	/*                       */
#define MAC_MCCFG_PMC	0x00000004	/*                   */

/*                            
  
                                                                  
                                         
 */
#define MAC_XIFCFG_OE	0x00000001	/*                             */
#define MAC_XIFCFG_LBCK	0x00000002	/*                    */
#define MAC_XIFCFG_DISE	0x00000004	/*                           */
#define MAC_XIFCFG_GMII	0x00000008	/*                            */
#define MAC_XIFCFG_MBOE	0x00000010	/*                         */
#define MAC_XIFCFG_LLED	0x00000020	/*                             */
#define MAC_XIFCFG_FLED	0x00000040	/*                              */

/*                                                                    
                                                                       
                                                                     
                                                                        
                                                   
  
                                                           
  
                          
 */

/*                                                                  
                                   
  
                                                           
  
                          
 */

/*                                                                   
                                   
  
                                                           
  
                          
 */

/*                                                               
                                                                     
                       
  
                          
 */

/*                                                                 
                                                                    
                                      
  
                          
 */

/*                                       
  
                                                                      
                                                                  
                                                                    
                                             
  
                                
 */
#define MAC_MAXFSZ_MFS	0x00007fff	/*                 */
#define MAC_MAXFSZ_MBS	0x7fff0000	/*                 */

/*                                                                         
                                                                     
                                                     
  
                          
 */

/*                                                                  
                                       
  
                          
 */

/*                                                                   
                                                                      
                                                                      
                                                                
                                                                   
                          
  
                          
 */

/*                                                               
                                                                     
                                                                    
                                                                  
           
  
                            
 */

/*                                                            
                                                              
                                                                  
                 
  
                                                           
                                 
  
                                                              
                                                                
                  
  
                                                               
                
                                
                                
                                
 */

/*                                                             
                                                              
                                                                  
                                                                 
                                                                 
                     
 */

/*                                                                 
                                                             
 */

/*                                                              
                                                                
                                                                 
                                                                       
                 
 */

/*                                                               
                                                                
                                                            
                          
 */

/*                                                                     
                                                                
                                                                        
                    
 */

/*               */
#define MIF_BBCLK	0x6200UL	/*                     */
#define MIF_BBDATA	0x6204UL	/*                    */
#define MIF_BBOENAB	0x6208UL	/*                            */
#define MIF_FRAME	0x620CUL	/*                           */
#define MIF_CFG		0x6210UL	/*                            */
#define MIF_MASK	0x6214UL	/*                    */
#define MIF_STATUS	0x6218UL	/*                      */
#define MIF_SMACHINE	0x621CUL	/*                            */

/*                                                                 
                                                                     
                                                                     
                                                                    
                                                                    
                                                                    
             
 */

/*                                                                
                                                                   
                                                                        
                                                                       
                          
 */

/*                                                                   
                                                                      
                                                                        
                                                                         
                                                                      
                                                                      
                                                                          
                                                                        
 */

/*                                                                         
              
 */
#define MIF_CFG_PSELECT	0x00000001	/*                            */
#define MIF_CFG_POLL	0x00000002	/*                          */
#define MIF_CFG_BBMODE	0x00000004	/*                         */
#define MIF_CFG_PRADDR	0x000000f8	/*                            */
#define MIF_CFG_MDI0	0x00000100	/*                            */
#define MIF_CFG_MDI1	0x00000200	/*                            */
#define MIF_CFG_PPADDR	0x00007c00	/*                       */

/*                                                                    
                                                                       
                                                                        
                                                                
                                                                      
                 
 */
#define MIF_FRAME_ST	0xc0000000	/*                 */
#define MIF_FRAME_OP	0x30000000	/*          */
#define MIF_FRAME_PHYAD	0x0f800000	/*               */
#define MIF_FRAME_REGAD	0x007c0000	/*                   */
#define MIF_FRAME_TAMSB	0x00020000	/*                  */
#define MIF_FRAME_TALSB	0x00010000	/*                  */
#define MIF_FRAME_DATA	0x0000ffff	/*                      */

/*                                                                   
                                                                      
           
 */
#define MIF_STATUS_DATA	0xffff0000	/*                        */
#define MIF_STATUS_STAT	0x0000ffff	/*                         */

/*                                                                   
                                                                   
                
 */

/*                         */
#define PCS_MIICTRL	0x9000UL	/*                          */
#define PCS_MIISTAT	0x9004UL	/*                         */
#define PCS_MIIADV	0x9008UL	/*                           */
#define PCS_MIILP	0x900CUL	/*                              */
#define PCS_CFG		0x9010UL	/*                            */
#define PCS_SMACHINE	0x9014UL	/*                            */
#define PCS_ISTAT	0x9018UL	/*                          */
#define PCS_DMODE	0x9050UL	/*                        */
#define PCS_SCTRL	0x9054UL	/*                            */
#define PCS_SOS		0x9058UL	/*                          */
#define PCS_SSTATE	0x905CUL	/*                          */

/*                           */
#define PCS_MIICTRL_SPD	0x00000040	/*                             */
#define PCS_MIICTRL_CT	0x00000080	/*                         */
#define PCS_MIICTRL_DM	0x00000100	/*                         */
#define PCS_MIICTRL_RAN	0x00000200	/*                              */
#define PCS_MIICTRL_ISO	0x00000400	/*                              */
#define PCS_MIICTRL_PD	0x00000800	/*                              */
#define PCS_MIICTRL_ANE	0x00001000	/*                  */
#define PCS_MIICTRL_SS	0x00002000	/*                              */
#define PCS_MIICTRL_WB	0x00004000	/*                             
                               
      */
#define PCS_MIICTRL_RST	0x00008000	/*                           */

/*                          */
#define PCS_MIISTAT_EC	0x00000001	/*                              */
#define PCS_MIISTAT_JD	0x00000002	/*                             */
#define PCS_MIISTAT_LS	0x00000004	/*                          */
#define PCS_MIISTAT_ANA	0x00000008	/*                            */
#define PCS_MIISTAT_RF	0x00000010	/*                */
#define PCS_MIISTAT_ANC	0x00000020	/*                    */
#define PCS_MIISTAT_ES	0x00000100	/*                           */

/*                                 */
#define PCS_MIIADV_FD	0x00000020	/*                       */
#define PCS_MIIADV_HD	0x00000040	/*                       */
#define PCS_MIIADV_SP	0x00000080	/*                           */
#define PCS_MIIADV_AP	0x00000100	/*                            */
#define PCS_MIIADV_RF	0x00003000	/*                */
#define PCS_MIIADV_ACK	0x00004000	/*             */
#define PCS_MIIADV_NP	0x00008000	/*                       */

/*                                                                     
                                                                         
                                                                 
 */

/*                             */
#define PCS_CFG_ENABLE	0x00000001	/*                            
                                  
      */
#define PCS_CFG_SDO	0x00000002	/*                        */
#define PCS_CFG_SDL	0x00000004	/*                          */
#define PCS_CFG_JS	0x00000018	/*              
                            
                                       
                                      
                    
      */
#define PCS_CFG_TO	0x00000020	/*                              */

/*                                                               
             
 */
#define PCS_ISTAT_LSC	0x00000004	/*                     */

/*                         */
#define PCS_DMODE_SM	0x00000001	/*                            */
#define PCS_DMODE_ESM	0x00000002	/*                       */
#define PCS_DMODE_MGM	0x00000004	/*                */
#define PCS_DMODE_GMOE	0x00000008	/*                     */

/*                            
  
                                                                 
 */
#define PCS_SCTRL_LOOP	0x00000001	/*                  */
#define PCS_SCTRL_ESCD	0x00000002	/*                            */
#define PCS_SCTRL_LOCK	0x00000004	/*                         */
#define PCS_SCTRL_EMP	0x00000018	/*                        */
#define PCS_SCTRL_STEST	0x000001c0	/*                     */
#define PCS_SCTRL_PDWN	0x00000200	/*                      */
#define PCS_SCTRL_RXZ	0x00000c00	/*                        */
#define PCS_SCTRL_RXP	0x00003000	/*                        */
#define PCS_SCTRL_TXZ	0x0000c000	/*                        */
#define PCS_SCTRL_TXP	0x00030000	/*                        */

/*                                                                        
                                                                   
             
 */
#define PCS_SOS_PADDR	0x00000003	/*                */

/*                  */
#define PROM_START	0x100000UL	/*                              */
#define PROM_SIZE	0x0fffffUL	/*               */
#define PROM_END	0x200000UL	/*              */

/*                                    */

#define BMCR_SPD2	0x0040		/*                           */
#define LPA_PAUSE	0x0400

/*                                                  */

/*                                         */
#define MII_BCM5201_INTERRUPT			0x1A
#define MII_BCM5201_INTERRUPT_INTENABLE		0x4000

#define MII_BCM5201_AUXMODE2			0x1B
#define MII_BCM5201_AUXMODE2_LOWPOWER		0x0008

#define MII_BCM5201_MULTIPHY                    0x1E

/*                                    */
#define MII_BCM5201_MULTIPHY_SERIALMODE         0x0002
#define MII_BCM5201_MULTIPHY_SUPERISOLATE       0x0008

/*                                         */
#define MII_BCM5400_GB_CONTROL			0x09
#define MII_BCM5400_GB_CONTROL_FULLDUPLEXCAP	0x0200

/*                                 */
#define MII_BCM5400_AUXCONTROL                  0x18
#define MII_BCM5400_AUXCONTROL_PWR10BASET       0x0004

/*                                */
#define MII_BCM5400_AUXSTATUS                   0x19
#define MII_BCM5400_AUXSTATUS_LINKMODE_MASK     0x0700
#define MII_BCM5400_AUXSTATUS_LINKMODE_SHIFT    8

/*                                                            
                                                          
  
                                                                    
                                                                    
                                                                   
                                                                    
  
                                                                 
                                                                   
                                         
 */
struct gem_txd {
	__le64	control_word;
	__le64	buffer;
};

#define TXDCTRL_BUFSZ	0x0000000000007fffULL	/*              */
#define TXDCTRL_CSTART	0x00000000001f8000ULL	/*                   */
#define TXDCTRL_COFF	0x000000001fe00000ULL	/*                   */
#define TXDCTRL_CENAB	0x0000000020000000ULL	/*              */
#define TXDCTRL_EOF	0x0000000040000000ULL	/*               */
#define TXDCTRL_SOF	0x0000000080000000ULL	/*                */
#define TXDCTRL_INTME	0x0000000100000000ULL	/*                */
#define TXDCTRL_NOCRC	0x0000000200000000ULL	/*                */

/*                                                              
                                                                    
                                                                   
                               
  
                                                                     
                                                                      
                    
  
                                                                       
                                                                     
                                                                    
                                                                     
                                                                    
                                                            
  
                                                                      
                                                                     
                                                                   
                                                              
 */
struct gem_rxd {
	__le64	status_word;
	__le64	buffer;
};

#define RXDCTRL_TCPCSUM	0x000000000000ffffULL	/*                 */
#define RXDCTRL_BUFSZ	0x000000007fff0000ULL	/*              */
#define RXDCTRL_OWN	0x0000000080000000ULL	/*                     */
#define RXDCTRL_HASHVAL	0x0ffff00000000000ULL	/*             */
#define RXDCTRL_HPASS	0x1000000000000000ULL	/*                    */
#define RXDCTRL_ALTMAC	0x2000000000000000ULL	/*                 */
#define RXDCTRL_BAD	0x4000000000000000ULL	/*                   */

#define RXDCTRL_FRESH(gp)	\
	((((RX_BUF_ALLOC_SIZE(gp) - RX_OFFSET) << 16) & RXDCTRL_BUFSZ) | \
	 RXDCTRL_OWN)

#define TX_RING_SIZE 128
#define RX_RING_SIZE 128

#if TX_RING_SIZE == 32
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_32
#elif TX_RING_SIZE == 64
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_64
#elif TX_RING_SIZE == 128
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_128
#elif TX_RING_SIZE == 256
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_256
#elif TX_RING_SIZE == 512
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_512
#elif TX_RING_SIZE == 1024
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_1K
#elif TX_RING_SIZE == 2048
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_2K
#elif TX_RING_SIZE == 4096
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_4K
#elif TX_RING_SIZE == 8192
#define TXDMA_CFG_BASE	TXDMA_CFG_RINGSZ_8K
#else
#error TX_RING_SIZE value is illegal...
#endif

#if RX_RING_SIZE == 32
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_32
#elif RX_RING_SIZE == 64
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_64
#elif RX_RING_SIZE == 128
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_128
#elif RX_RING_SIZE == 256
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_256
#elif RX_RING_SIZE == 512
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_512
#elif RX_RING_SIZE == 1024
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_1K
#elif RX_RING_SIZE == 2048
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_2K
#elif RX_RING_SIZE == 4096
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_4K
#elif RX_RING_SIZE == 8192
#define RXDMA_CFG_BASE	RXDMA_CFG_RINGSZ_8K
#else
#error RX_RING_SIZE is illegal...
#endif

#define NEXT_TX(N)	(((N) + 1) & (TX_RING_SIZE - 1))
#define NEXT_RX(N)	(((N) + 1) & (RX_RING_SIZE - 1))

#define TX_BUFFS_AVAIL(GP)					\
	(((GP)->tx_old <= (GP)->tx_new) ?			\
	  (GP)->tx_old + (TX_RING_SIZE - 1) - (GP)->tx_new :	\
	  (GP)->tx_old - (GP)->tx_new - 1)

#define RX_OFFSET          2
#define RX_BUF_ALLOC_SIZE(gp)	((gp)->rx_buf_sz + 28 + RX_OFFSET + 64)

#define RX_COPY_THRESHOLD  256

#if TX_RING_SIZE < 128
#define INIT_BLOCK_TX_RING_SIZE		128
#else
#define INIT_BLOCK_TX_RING_SIZE		TX_RING_SIZE
#endif

#if RX_RING_SIZE < 128
#define INIT_BLOCK_RX_RING_SIZE		128
#else
#define INIT_BLOCK_RX_RING_SIZE		RX_RING_SIZE
#endif

struct gem_init_block {
	struct gem_txd	txd[INIT_BLOCK_TX_RING_SIZE];
	struct gem_rxd	rxd[INIT_BLOCK_RX_RING_SIZE];
};

enum gem_phy_type {
	phy_mii_mdio0,
	phy_mii_mdio1,
	phy_serialink,
	phy_serdes,
};

enum link_state {
	link_down = 0,	/*                     */
	link_aneg,	/*                     */
	link_force_try,	/*                       */
	link_force_ret,	/*                                      */
	link_force_ok,	/*                     */
	link_up		/*            */
};

struct gem {
	void __iomem		*regs;
	int			rx_new, rx_old;
	int			tx_new, tx_old;

	unsigned int has_wol : 1;	/*                           */
	unsigned int asleep_wol : 1;	/*                             */

	int			cell_enabled;
	u32			msg_enable;
	u32			status;

	struct napi_struct	napi;

	int			tx_fifo_sz;
	int			rx_fifo_sz;
	int			rx_pause_off;
	int			rx_pause_on;
	int			rx_buf_sz;
	u64			pause_entered;
	u16			pause_last_time_recvd;
	u32			mac_rx_cfg;
	u32			swrst_base;

	int			want_autoneg;
	int			last_forced_speed;
	enum link_state		lstate;
	struct timer_list	link_timer;
	int			timer_ticks;
	int			wake_on_lan;
	struct work_struct	reset_task;
	volatile int		reset_task_pending;

	enum gem_phy_type	phy_type;
	struct mii_phy		phy_mii;
	int			mii_phy_addr;

	struct gem_init_block	*init_block;
	struct sk_buff		*rx_skbs[RX_RING_SIZE];
	struct sk_buff		*tx_skbs[TX_RING_SIZE];
	dma_addr_t		gblock_dvma;

	struct pci_dev		*pdev;
	struct net_device	*dev;
#if defined(CONFIG_PPC_PMAC) || defined(CONFIG_SPARC)
	struct device_node	*of_node;
#endif
};

#define found_mii_phy(gp) ((gp->phy_type == phy_mii_mdio0 || gp->phy_type == phy_mii_mdio1) && \
			   gp->phy_mii.def && gp->phy_mii.def->ops)

#endif /*           */
