/*                                  
           
                                   */
#ifndef	__ADAPTER_H__
#define	__ADAPTER_H__

#define MAX_FRAGMENTEDIP_CLASSIFICATION_ENTRIES 256
#include "Debug.h"

struct _LEADER
{
	USHORT 	Vcid;
	USHORT 	PLength;
	UCHAR  	Status;
	UCHAR	Unused[3];
}__attribute__((packed));
typedef struct _LEADER LEADER,*PLEADER;

struct _PACKETTOSEND
{
	LEADER	Leader;
	UCHAR	ucPayload;
}__attribute__((packed));
typedef struct _PACKETTOSEND PACKETTOSEND, *PPACKETTOSEND;


struct _CONTROL_PACKET
{
	PVOID	ControlBuff;
	UINT	ControlBuffLen;
	struct _CONTROL_PACKET* next;
}__attribute__((packed));
typedef struct _CONTROL_PACKET CONTROL_PACKET,*PCONTROL_PACKET;


struct link_request
{
	LEADER	Leader;
	UCHAR	szData[4];
}__attribute__((packed));
typedef struct link_request LINK_REQUEST, *PLINK_REQUEST;


//                                 
typedef struct _ADD_CONNECTION
{
    ULONG 		SrcIpAddressCount;
    ULONG 		SrcIpAddress[MAX_CONNECTIONS];
    ULONG 		SrcIpMask[MAX_CONNECTIONS];

    ULONG 		DestIpAddressCount;
    ULONG 		DestIpAddress[MAX_CONNECTIONS];
    ULONG 		DestIpMask[MAX_CONNECTIONS];

    USHORT 		SrcPortBegin;
    USHORT 		SrcPortEnd;

    USHORT 		DestPortBegin;
    USHORT 		DestPortEnd;

    UCHAR 		SrcTOS;
    UCHAR 		SrcProtocol;
} ADD_CONNECTION,*PADD_CONNECTION;


typedef struct _CLASSIFICATION_RULE
{
	UCHAR		ucIPSrcAddrLen;
	UCHAR       ucIPSrcAddr[32];
	UCHAR		ucIPDestAddrLen;
	UCHAR       ucIPDestAddr[32];
	UCHAR		ucSrcPortRangeLen;
	UCHAR		ucSrcPortRange[4];
	UCHAR		ucDestPortRangeLen;
	UCHAR		ucDestPortRange[4];
	USHORT		usVcid;
} CLASSIFICATION_RULE,*PCLASSIFICATION_RULE;

typedef struct _CLASSIFICATION_ONLY
{
    USHORT 		usVcid;
    ULONG  		DestIpAddress;
    ULONG  		DestIpMask;
    USHORT 		usPortLo;
    USHORT 		usPortHi;
    BOOLEAN		bIpVersion;
    UCHAR		ucDestinationAddress[16];
} CLASSIFICATION_ONLY, *PCLASSIFICATION_ONLY;


#define MAX_IP_RANGE_LENGTH 4
#define MAX_PORT_RANGE 4
#define MAX_PROTOCOL_LENGTH   32
#define IPV6_ADDRESS_SIZEINBYTES 0x10

typedef union _U_IP_ADDRESS
{
    struct
	{
		ULONG				ulIpv4Addr[MAX_IP_RANGE_LENGTH];//                       
		ULONG               ulIpv4Mask[MAX_IP_RANGE_LENGTH];//                            
	};
	struct
	{
		ULONG				ulIpv6Addr[MAX_IP_RANGE_LENGTH * 4];//                       
		ULONG               ulIpv6Mask[MAX_IP_RANGE_LENGTH * 4];//                            

	};
	struct
	{
		UCHAR				ucIpv4Address[MAX_IP_RANGE_LENGTH * IP_LENGTH_OF_ADDRESS];
		UCHAR				ucIpv4Mask[MAX_IP_RANGE_LENGTH * IP_LENGTH_OF_ADDRESS];
	};
	struct
	{
		UCHAR				ucIpv6Address[MAX_IP_RANGE_LENGTH * IPV6_ADDRESS_SIZEINBYTES];
		UCHAR				ucIpv6Mask[MAX_IP_RANGE_LENGTH * IPV6_ADDRESS_SIZEINBYTES];
	};
}U_IP_ADDRESS;
struct _packet_info;

typedef struct _S_HDR_SUPRESSION_CONTEXTINFO
{

	UCHAR      ucaHdrSupressionInBuf[MAX_PHS_LENGTHS]; //                                                    
	UCHAR      ucaHdrSupressionOutBuf[MAX_PHS_LENGTHS + PHSI_LEN]; //                                                   

}S_HDR_SUPRESSION_CONTEXTINFO;


typedef struct _S_CLASSIFIER_RULE
{
	ULONG			ulSFID;
	UCHAR           ucReserved[2];
	B_UINT16        uiClassifierRuleIndex;
	BOOLEAN			bUsed;
	USHORT			usVCID_Value;
	B_UINT8         u8ClassifierRulePriority; //                                            
	U_IP_ADDRESS	stSrcIpAddress;
	UCHAR           ucIPSourceAddressLength;//                        

	U_IP_ADDRESS    stDestIpAddress;
    UCHAR           ucIPDestinationAddressLength;//                             
    UCHAR           ucIPTypeOfServiceLength;//                      
    UCHAR           ucTosLow;//       
    UCHAR           ucTosHigh;//        
	UCHAR           ucTosMask;//        

    UCHAR           ucProtocolLength;//               
    UCHAR           ucProtocol[MAX_PROTOCOL_LENGTH];//               
    USHORT			usSrcPortRangeLo[MAX_PORT_RANGE];
	USHORT			usSrcPortRangeHi[MAX_PORT_RANGE];
    UCHAR           ucSrcPortRangeLength;

    USHORT			usDestPortRangeLo[MAX_PORT_RANGE];
	USHORT			usDestPortRangeHi[MAX_PORT_RANGE];
    UCHAR           ucDestPortRangeLength;

	BOOLEAN			bProtocolValid;
	BOOLEAN			bTOSValid;
	BOOLEAN			bDestIpValid;
	BOOLEAN			bSrcIpValid;

	//                   
	UCHAR			ucDirection;
	BOOLEAN         bIpv6Protocol;
	UINT32          u32PHSRuleID;
	S_PHS_RULE 	    sPhsRule;
	UCHAR			u8AssociatedPHSI;

	//                                
	UCHAR		ucEthCSSrcMACLen;
	UCHAR		au8EThCSSrcMAC[MAC_ADDRESS_SIZE];
	UCHAR		au8EThCSSrcMACMask[MAC_ADDRESS_SIZE];
	UCHAR		ucEthCSDestMACLen;
	UCHAR		au8EThCSDestMAC[MAC_ADDRESS_SIZE];
	UCHAR		au8EThCSDestMACMask[MAC_ADDRESS_SIZE];
	UCHAR		ucEtherTypeLen;
	UCHAR		au8EthCSEtherType[NUM_ETHERTYPE_BYTES];
	UCHAR		usUserPriority[2];
	USHORT		usVLANID;
	USHORT		usValidityBitMap;
}S_CLASSIFIER_RULE;
//                                                    

typedef struct _S_FRAGMENTED_PACKET_INFO
{
	BOOLEAN				bUsed;
	ULONG		        ulSrcIpAddress;
	USHORT 				usIpIdentification;
	S_CLASSIFIER_RULE 	*pstMatchedClassifierEntry;
	BOOLEAN				bOutOfOrderFragment;
}S_FRAGMENTED_PACKET_INFO,*PS_FRAGMENTED_PACKET_INFO;

struct _packet_info
{
	//                             
   	ULONG			ulSFID;
   	USHORT			usVCID_Value;
	UINT			uiThreshold;
	//                                                    
	B_UINT8     	u8TrafficPriority;

	BOOLEAN			bValid;
   	BOOLEAN     	bActive;
	BOOLEAN			bActivateRequestSent;

	B_UINT8			u8QueueType;//          

	UINT			uiMaxBucketSize;//                                        
	UINT			uiCurrentQueueDepthOnTarget;
	UINT			uiCurrentBytesOnHost;
	UINT			uiCurrentPacketsOnHost;
	UINT			uiDroppedCountBytes;
	UINT			uiDroppedCountPackets;
	UINT			uiSentBytes;
	UINT			uiSentPackets;
	UINT			uiCurrentDrainRate;
	UINT			uiThisPeriodSentBytes;
	LARGE_INTEGER	liDrainCalculated;
	UINT			uiCurrentTokenCount;
	LARGE_INTEGER 	liLastUpdateTokenAt;
	UINT			uiMaxAllowedRate;
	UINT			NumOfPacketsSent;
	UCHAR			ucDirection;
	USHORT			usCID;
	S_MIBS_EXTSERVICEFLOW_PARAMETERS	stMibsExtServiceFlowTable;
	UINT			uiCurrentRxRate;
	UINT			uiThisPeriodRxBytes;
	UINT			uiTotalRxBytes;
	UINT			uiTotalTxBytes;
	UINT			uiPendedLast;
	UCHAR			ucIpVersion;

	union
	{
		struct
		{
			struct sk_buff*	   FirstTxQueue;
			struct sk_buff*	   LastTxQueue;
		};
		struct
		{
			struct sk_buff*	   ControlHead;
			struct sk_buff*	   ControlTail;
		};
	};
	BOOLEAN		bProtocolValid;
	BOOLEAN		bTOSValid;
	BOOLEAN		bDestIpValid;
	BOOLEAN		bSrcIpValid;

	BOOLEAN		bActiveSet;
	BOOLEAN		bAdmittedSet;
	BOOLEAN		bAuthorizedSet;
	BOOLEAN		bClassifierPriority;
	UCHAR		ucServiceClassName[MAX_CLASS_NAME_LENGTH];
	BOOLEAN     bHeaderSuppressionEnabled;
	spinlock_t	SFQueueLock;
	void 		*pstSFIndication;
	struct timeval	stLastUpdateTokenAt;
	atomic_t 	uiPerSFTxResourceCount;
	UINT		uiMaxLatency;
	UCHAR 	bIPCSSupport;
	UCHAR 	bEthCSSupport;
};
typedef struct _packet_info PacketInfo;


typedef struct _PER_TARANG_DATA
{
    struct _PER_TARANG_DATA * next;
    struct _MINI_ADAPTER * Adapter;
    struct sk_buff*     RxAppControlHead;
    struct sk_buff*     RxAppControlTail;
    volatile INT        AppCtrlQueueLen;
    BOOLEAN             MacTracingEnabled;
	BOOLEAN				bApplicationToExit;
	S_MIBS_DROPPED_APP_CNTRL_MESSAGES stDroppedAppCntrlMsgs;
	ULONG 				RxCntrlMsgBitMask;
} PER_TARANG_DATA, *PPER_TARANG_DATA;


#ifdef REL_4_1
typedef struct _TARGET_PARAMS
{
      B_UINT32 m_u32CfgVersion;

      //                        
      B_UINT32 m_u32CenterFrequency;
      B_UINT32 m_u32BandAScan;
      B_UINT32 m_u32BandBScan;
      B_UINT32 m_u32BandCScan;

      //           
      B_UINT32 m_u32minGrantsize;	//                                
      B_UINT32 m_u32PHSEnable;

      //          
      B_UINT32 m_u32HoEnable;
      B_UINT32 m_u32HoReserved1;
      B_UINT32 m_u32HoReserved2;

	//                     
      B_UINT32 m_u32MimoEnable;
      B_UINT32 m_u32SecurityEnable;
	/*
                                
                                
     */
      B_UINT32 m_u32PowerSavingModesEnable;
	  /*                          
                                                    
                                              
                                                */
      B_UINT32 m_u32PowerSavingModeOptions;

      B_UINT32 m_u32ArqEnable;

      //                                                     
      B_UINT32 m_u32HarqEnable;
       //                      
       B_UINT32  m_u32EEPROMFlag;
       /*                          
                                       
                                  
        */
       //                
      B_UINT32   m_u32Customize;
      B_UINT32   m_u32ConfigBW;  /*       */
      B_UINT32   m_u32ShutDownTimer;


      B_UINT32  m_u32RadioParameter;
      B_UINT32  m_u32PhyParameter1;
      B_UINT32  m_u32PhyParameter2;
      B_UINT32  m_u32PhyParameter3;

	/*                   
                                            
                                 
                              
                                
                                   
     */
    B_UINT32	  m_u32TestOptions;

	B_UINT32 m_u32MaxMACDataperDLFrame;
	B_UINT32 m_u32MaxMACDataperULFrame;

	B_UINT32 m_u32Corr2MacFlags;

    //                     
    B_UINT32 HostDrvrConfig1;
    B_UINT32 HostDrvrConfig2;
    B_UINT32 HostDrvrConfig3;
    B_UINT32 HostDrvrConfig4;
    B_UINT32 HostDrvrConfig5;
    B_UINT32 HostDrvrConfig6;
    B_UINT32 m_u32SegmentedPUSCenable;

	//                                                    
	//                                            
	B_UINT32 m_u32BandAMCEnable;
} STARGETPARAMS, *PSTARGETPARAMS;
#endif

typedef struct _STTARGETDSXBUFFER
{
    ULONG ulTargetDsxBuffer;
    B_UINT16 tid;
    BOOLEAN valid;
}STTARGETDSXBUFFER, *PSTTARGETDSXBUFFER;

typedef INT (*FP_FLASH_WRITE)(struct _MINI_ADAPTER*,UINT,PVOID);

typedef INT (*FP_FLASH_WRITE_STATUS)(struct _MINI_ADAPTER*,UINT,PVOID);

/* 
                             
*/
struct _MINI_ADAPTER
{
	struct _MINI_ADAPTER *next;
	struct net_device	*dev;
	u32			msg_enable;

	CHAR                *caDsxReqResp;
	atomic_t		ApplicationRunning;
	volatile INT		CtrlQueueLen;
	atomic_t            	AppCtrlQueueLen;
	BOOLEAN             	AppCtrlQueueOverFlow;
	atomic_t		CurrentApplicationCount;
	atomic_t 		RegisteredApplicationCount;
	BOOLEAN		  	LinkUpStatus;
	BOOLEAN		    	TimerActive;
	u32			StatisticsPointer;
	struct sk_buff		*RxControlHead;
	struct sk_buff		*RxControlTail;

	struct semaphore	RxAppControlQueuelock;
	struct semaphore	fw_download_sema;

	PPER_TARANG_DATA    pTarangs;
	spinlock_t			control_queue_lock;
	wait_queue_head_t	process_read_wait_queue;

	//                                                       
	//                                        
	atomic_t		    TotalPacketCount;
	atomic_t		    TxPktAvail;

	//                                                       
	atomic_t		    CurrNumFreeTxDesc;
	//                                      
	USHORT				PrevNumRecvDescs;
	USHORT				CurrNumRecvDescs;
	UINT				u32TotalDSD;
	PacketInfo		    PackInfo[NO_OF_QUEUES];
	S_CLASSIFIER_RULE	astClassifierTable[MAX_CLASSIFIERS];
	BOOLEAN			    TransferMode;

	/*                                    */
	BOOLEAN			    bETHCSEnabled;

	ULONG			    BEBucketSize;
	ULONG			    rtPSBucketSize;
	UCHAR			    LinkStatus;
	BOOLEAN			    AutoLinkUp;
	BOOLEAN			    AutoSyncup;

	int				major;
	int				minor;
	wait_queue_head_t 	tx_packet_wait_queue;
	wait_queue_head_t 	process_rx_cntrlpkt;
	atomic_t			process_waiting;
	BOOLEAN 			fw_download_done;

	char 				*txctlpacket[MAX_CNTRL_PKTS];
	atomic_t			cntrlpktCnt ;
	atomic_t			index_app_read_cntrlpkt;
	atomic_t			index_wr_txcntrlpkt;
	atomic_t			index_rd_txcntrlpkt;
	UINT				index_datpkt;
	struct semaphore 	rdmwrmsync;

	STTARGETDSXBUFFER	astTargetDsxBuffer[MAX_TARGET_DSX_BUFFERS];
	ULONG			ulFreeTargetBufferCnt;
	ULONG              	ulCurrentTargetBuffer;
	ULONG              	ulTotalTargetBuffersAvailable;

	unsigned long 		chip_id;

	wait_queue_head_t 	lowpower_mode_wait_queue;

	BOOLEAN			bFlashBoot;
	BOOLEAN			bBinDownloaded;
	BOOLEAN			bCfgDownloaded;
	BOOLEAN			bSyncUpRequestSent;
	USHORT			usBestEffortQueueIndex;

	wait_queue_head_t 	ioctl_fw_dnld_wait_queue;
	BOOLEAN				waiting_to_fw_download_done;
	pid_t				fw_download_process_pid;
	PSTARGETPARAMS		pstargetparams;
	BOOLEAN				device_removed;
	BOOLEAN				DeviceAccess;
	BOOLEAN				bIsAutoCorrectEnabled;
	BOOLEAN				bDDRInitDone;
	INT				DDRSetting;
	ULONG				ulPowerSaveMode;
	spinlock_t			txtransmitlock;
	B_UINT8				txtransmit_running;
	/*                                    */
	struct task_struct 	*control_packet_handler;
	/*                                  */
	struct task_struct 	*transmit_packet_thread;

	/*                        */
	LED_INFO_STRUCT		LEDInfo;

	/*                               */
	LedEventInfo_t		DriverState;
	/*                    */
	PVOID				pvInterfaceAdapter;
	int (*bcm_file_download)( PVOID,
                        struct file *,
                        unsigned int);
	int (*bcm_file_readback_from_chip)( PVOID,
                        struct file *,
                        unsigned int);
	INT (*interface_rdm)(PVOID,
			UINT ,
			PVOID ,
			INT);
	INT (*interface_wrm)(PVOID,
			UINT ,
			PVOID ,
			INT);
	int (*interface_transmit)(PVOID, PVOID , UINT);
	BOOLEAN			IdleMode;
	BOOLEAN			bDregRequestSentInIdleMode;
	BOOLEAN			bTriedToWakeUpFromlowPowerMode;
	BOOLEAN			bShutStatus;
	BOOLEAN			bWakeUpDevice;
	unsigned int	usIdleModePattern;
	//                                     
	BOOLEAN			bLinkDownRequested;

	int 			downloadDDR;
	PHS_DEVICE_EXTENSION stBCMPhsContext;
	S_HDR_SUPRESSION_CONTEXTINFO	stPhsTxContextInfo;
	uint8_t			ucaPHSPktRestoreBuf[2048];
	uint8_t			bPHSEnabled;
	BOOLEAN			AutoFirmDld;
	BOOLEAN         bMipsConfig;
	BOOLEAN         bDPLLConfig;
	UINT32			aTxPktSizeHist[MIBS_MAX_HIST_ENTRIES];
	UINT32			aRxPktSizeHist[MIBS_MAX_HIST_ENTRIES];
	S_FRAGMENTED_PACKET_INFO astFragmentedPktClassifierTable[MAX_FRAGMENTEDIP_CLASSIFICATION_ENTRIES];
	atomic_t		uiMBupdate;
	UINT32			PmuMode;
	NVM_TYPE		eNVMType;
	UINT			uiSectorSize;
	UINT			uiSectorSizeInCFG;
	BOOLEAN			bSectorSizeOverride;
	BOOLEAN			bStatusWrite;
	UINT			uiNVMDSDSize;
	UINT			uiVendorExtnFlag;
	 //                                                         
	 //                                                                           
	UINT			ulFlashCalStart;
	ULONG                   ulFlashControlSectionStart;
	ULONG                   ulFlashWriteSize;
	ULONG                   ulFlashID;
	FP_FLASH_WRITE          fpFlashWrite;
	FP_FLASH_WRITE_STATUS   fpFlashWriteWithStatusCheck;


	struct semaphore	NVMRdmWrmLock;

	struct device *pstCreatedClassDevice;

//                              
	PFLASH2X_CS_INFO psFlash2xCSInfo;
	PFLASH_CS_INFO psFlashCSInfo ;
	PFLASH2X_VENDORSPECIFIC_INFO psFlash2xVendorInfo;
	UINT uiFlashBaseAdd; //                   
	UINT uiActiveISOOffset; //                                            
	FLASH2X_SECTION_VAL eActiveISO; //                      
	FLASH2X_SECTION_VAL eActiveDSD;	//                                         
	UINT uiActiveDSDOffsetAtFwDld;  //                                                   
	UINT uiFlashLayoutMajorVersion ;
	UINT uiFlashLayoutMinorVersion;
	BOOLEAN bAllDSDWriteAllow ;
	BOOLEAN bSigCorrupted ;
	//                                                                                                      
	BOOLEAN bHeaderChangeAllowed ;
	INT SelectedChip ;
	BOOLEAN bEndPointHalted;
	//                                                                                                      
	BOOLEAN bFlashRawRead;
	BOOLEAN			bPreparingForLowPowerMode ;
	BOOLEAN bDoSuspend ;
	UINT syscfgBefFwDld ;
	BOOLEAN StopAllXaction ;
	UINT32	liTimeSinceLastNetEntry; //                                               
	struct semaphore	LowPowerModeSync;
	ULONG	liDrainCalculated;
	UINT gpioBitMap;

    S_BCM_DEBUG_STATE stDebugState;

};
typedef struct _MINI_ADAPTER MINI_ADAPTER, *PMINI_ADAPTER;

#define GET_BCM_ADAPTER(net_dev)	netdev_priv(net_dev)

struct _ETH_HEADER_STRUC {
    UCHAR       au8DestinationAddress[6];
    UCHAR       au8SourceAddress[6];
    USHORT      u16Etype;
}__attribute__((packed));
typedef struct _ETH_HEADER_STRUC ETH_HEADER_STRUC, *PETH_HEADER_STRUC;


typedef struct FirmwareInfo
{
	void __user *	pvMappedFirmwareAddress;
	ULONG		u32FirmwareLength;
	ULONG		u32StartingAddress;
}__attribute__((packed)) FIRMWARE_INFO, *PFIRMWARE_INFO;

//                                          
extern struct net_device *gblpnetdev;
typedef struct _cntl_pkt{
	PMINI_ADAPTER 	Adapter;
	PLEADER 		PLeader;
}cntl_pkt;
typedef LINK_REQUEST CONTROL_MESSAGE;

typedef struct _DDR_SETTING
{
	UINT ulRegAddress;
	UINT ulRegValue;
}DDR_SETTING, *PDDR_SETTING;
typedef DDR_SETTING DDR_SET_NODE, *PDDR_SET_NODE;
INT
InitAdapter(PMINI_ADAPTER psAdapter);

//                                                                      
//                                    
//                                                                      

#define BCM_REQUEST_READ  0x2
#define BCM_REQUEST_WRITE 0x1
#define EP2_MPS_REG  0x0F0110A0
#define EP2_MPS 	 0x40

#define EP2_CFG_REG  0x0F0110A8
#define EP2_CFG_INT  0x27
#define EP2_CFG_BULK 0x25

#define EP4_MPS_REG  0x0F0110F0
#define EP4_MPS      0x8C

#define EP4_CFG_REG  0x0F0110F8

#define ISO_MPS_REG  0x0F0110C8
#define ISO_MPS      0x00000000


#define EP1 0
#define EP2 1
#define EP3 2
#define EP4 3
#define EP5 4
#define EP6 5


typedef enum eInterface_setting
{
	DEFAULT_SETTING_0  = 0,
	ALTERNATE_SETTING_1 = 1,
}INTERFACE_SETTING;

#endif	//             

