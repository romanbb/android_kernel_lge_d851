#ifndef __LINUX_ATALK_H__
#define __LINUX_ATALK_H__

#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/socket.h>

/*
                                  
  
                                                                        
                                      
 */
#define ATPORT_FIRST	1
#define ATPORT_RESERVED	128
#define ATPORT_LAST	254		/*                                */ 
#define ATADDR_ANYNET	(__u16)0
#define ATADDR_ANYNODE	(__u8)0
#define ATADDR_ANYPORT  (__u8)0
#define ATADDR_BCAST	(__u8)255
#define DDP_MAXSZ	587
#define DDP_MAXHOPS     15		/*                       */

#define SIOCATALKDIFADDR       (SIOCPROTOPRIVATE + 0)

struct atalk_addr {
	__be16	s_net;
	__u8	s_node;
};

struct sockaddr_at {
	__kernel_sa_family_t sat_family;
	__u8		  sat_port;
	struct atalk_addr sat_addr;
	char		  sat_zero[8];
};

struct atalk_netrange {
	__u8	nr_phase;
	__be16	nr_firstnet;
	__be16	nr_lastnet;
};

#ifdef __KERNEL__

#include <net/sock.h>

struct atalk_route {
	struct net_device  *dev;
	struct atalk_addr  target;
	struct atalk_addr  gateway;
	int		   flags;
	struct atalk_route *next;
};

/* 
                                           
                                                       
                         
                               
                                     
                                                 
 */
struct atalk_iface {
	struct net_device	*dev;
	struct atalk_addr	address;
	int			status;
#define ATIF_PROBE	1		/*                        */
#define ATIF_PROBE_FAIL	2		/*                */
	struct atalk_netrange	nets;
	struct atalk_iface	*next;
};
	
struct atalk_sock {
	/*                                                      */
	struct sock	sk;
	__be16		dest_net;
	__be16		src_net;
	unsigned char	dest_node;
	unsigned char	src_node;
	unsigned char	dest_port;
	unsigned char	src_port;
};

static inline struct atalk_sock *at_sk(struct sock *sk)
{
	return (struct atalk_sock *)sk;
}

struct ddpehdr {
	__be16	deh_len_hops;	/*                                         */
	__be16	deh_sum;
	__be16	deh_dnet;
	__be16	deh_snet;
	__u8	deh_dnode;
	__u8	deh_snode;
	__u8	deh_dport;
	__u8	deh_sport;
	/*                                                          */
};

static __inline__ struct ddpehdr *ddp_hdr(struct sk_buff *skb)
{
	return (struct ddpehdr *)skb_transport_header(skb);
}

/*                        */
struct elapaarp {
	__be16	hw_type;
#define AARP_HW_TYPE_ETHERNET		1
#define AARP_HW_TYPE_TOKENRING		2
	__be16	pa_type;
	__u8	hw_len;
	__u8	pa_len;
#define AARP_PA_ALEN			4
	__be16	function;
#define AARP_REQUEST			1
#define AARP_REPLY			2
#define AARP_PROBE			3
	__u8	hw_src[ETH_ALEN];
	__u8	pa_src_zero;
	__be16	pa_src_net;
	__u8	pa_src_node;
	__u8	hw_dst[ETH_ALEN];
	__u8	pa_dst_zero;
	__be16	pa_dst_net;
	__u8	pa_dst_node;
} __attribute__ ((packed));

static __inline__ struct elapaarp *aarp_hdr(struct sk_buff *skb)
{
	return (struct elapaarp *)skb_transport_header(skb);
}

/*                                                        */
#define AARP_EXPIRY_TIME	(5 * 60 * HZ)
/*                    */
#define AARP_HASH_SIZE		16
/*                                          */
#define AARP_TICK_TIME		(HZ / 5)
/*                                           */
#define AARP_RETRANSMIT_LIMIT	10
/*
                                                                         
                                        
 */
#define AARP_RESOLVE_TIME	(10 * HZ)

extern struct datalink_proto *ddp_dl, *aarp_dl;
extern void aarp_proto_init(void);

/*                      */

/*                                               */
static inline struct atalk_iface *atalk_find_dev(struct net_device *dev)
{
	return dev->atalk_ptr;
}

extern struct atalk_addr *atalk_find_dev_addr(struct net_device *dev);
extern struct net_device *atrtr_get_dev(struct atalk_addr *sa);
extern int		 aarp_send_ddp(struct net_device *dev,
				       struct sk_buff *skb,
				       struct atalk_addr *sa, void *hwaddr);
extern void		 aarp_device_down(struct net_device *dev);
extern void		 aarp_probe_network(struct atalk_iface *atif);
extern int 		 aarp_proxy_probe_network(struct atalk_iface *atif,
				     struct atalk_addr *sa);
extern void		 aarp_proxy_remove(struct net_device *dev,
					   struct atalk_addr *sa);

extern void		aarp_cleanup_module(void);

extern struct hlist_head atalk_sockets;
extern rwlock_t atalk_sockets_lock;

extern struct atalk_route *atalk_routes;
extern rwlock_t atalk_routes_lock;

extern struct atalk_iface *atalk_interfaces;
extern rwlock_t atalk_interfaces_lock;

extern struct atalk_route atrtr_default;

extern const struct file_operations atalk_seq_arp_fops;

extern int sysctl_aarp_expiry_time;
extern int sysctl_aarp_tick_time;
extern int sysctl_aarp_retransmit_limit;
extern int sysctl_aarp_resolve_time;

#ifdef CONFIG_SYSCTL
extern void atalk_register_sysctl(void);
extern void atalk_unregister_sysctl(void);
#else
#define atalk_register_sysctl()		do { } while(0)
#define atalk_unregister_sysctl()	do { } while(0)
#endif

#ifdef CONFIG_PROC_FS
extern int atalk_proc_init(void);
extern void atalk_proc_exit(void);
#else
#define atalk_proc_init()	({ 0; })
#define atalk_proc_exit()	do { } while(0)
#endif /*                */

#endif /*            */
#endif /*                   */