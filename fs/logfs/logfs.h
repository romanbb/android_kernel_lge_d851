/*
 * fs/logfs/logfs.h
 *
 * As should be obvious for Linux kernel code, license is GPLv2
 *
 * Copyright (c) 2005-2008 Joern Engel <joern@logfs.org>
 *
 * Private header for logfs.
 */
#ifndef FS_LOGFS_LOGFS_H
#define FS_LOGFS_LOGFS_H

#undef __CHECK_ENDIAN__
#define __CHECK_ENDIAN__

#include <linux/btree.h>
#include <linux/crc32.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/mempool.h>
#include <linux/pagemap.h>
#include <linux/mtd/mtd.h>
#include "logfs_abi.h"

#define LOGFS_DEBUG_SUPER	(0x0001)
#define LOGFS_DEBUG_SEGMENT	(0x0002)
#define LOGFS_DEBUG_JOURNAL	(0x0004)
#define LOGFS_DEBUG_DIR		(0x0008)
#define LOGFS_DEBUG_FILE	(0x0010)
#define LOGFS_DEBUG_INODE	(0x0020)
#define LOGFS_DEBUG_READWRITE	(0x0040)
#define LOGFS_DEBUG_GC		(0x0080)
#define LOGFS_DEBUG_GC_NOISY	(0x0100)
#define LOGFS_DEBUG_ALIASES	(0x0200)
#define LOGFS_DEBUG_BLOCKMOVE	(0x0400)
#define LOGFS_DEBUG_ALL		(0xffffffff)

#define LOGFS_DEBUG		(0x01)
/*
                                                                          
                       
 */
#ifndef LOGFS_DEBUG
#define LOGFS_DEBUG		(0)
#endif

#define log_cond(cond, fmt, arg...) do {	\
	if (cond)				\
		printk(KERN_DEBUG fmt, ##arg);	\
} while (0)

#define log_super(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_SUPER, fmt, ##arg)
#define log_segment(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_SEGMENT, fmt, ##arg)
#define log_journal(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_JOURNAL, fmt, ##arg)
#define log_dir(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_DIR, fmt, ##arg)
#define log_file(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_FILE, fmt, ##arg)
#define log_inode(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_INODE, fmt, ##arg)
#define log_readwrite(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_READWRITE, fmt, ##arg)
#define log_gc(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_GC, fmt, ##arg)
#define log_gc_noisy(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_GC_NOISY, fmt, ##arg)
#define log_aliases(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_ALIASES, fmt, ##arg)
#define log_blockmove(fmt, arg...) \
	log_cond(LOGFS_DEBUG & LOGFS_DEBUG_BLOCKMOVE, fmt, ##arg)

#define PG_pre_locked		PG_owner_priv_1
#define PagePreLocked(page)	test_bit(PG_pre_locked, &(page)->flags)
#define SetPagePreLocked(page)	set_bit(PG_pre_locked, &(page)->flags)
#define ClearPagePreLocked(page) clear_bit(PG_pre_locked, &(page)->flags)

/*                                                           */
#define LOGFS_LINK_MAX		(1<<30)

/*                      */
#define LOGFS_SB_FLAG_RO	0x0001
#define LOGFS_SB_FLAG_DIRTY	0x0002
#define LOGFS_SB_FLAG_OBJ_ALIAS	0x0004
#define LOGFS_SB_FLAG_SHUTDOWN	0x0008

/*                     */
#define WF_LOCK			0x01 /*                 */
#define WF_WRITE		0x02 /*             */
#define WF_DELETE		0x04 /*                  */

typedef u8 __bitwise level_t;
typedef u8 __bitwise gc_level_t;

#define LEVEL(level) ((__force level_t)(level))
#define GC_LEVEL(gc_level) ((__force gc_level_t)(gc_level))

#define SUBLEVEL(level) ( (void)((level) == LEVEL(1)),	\
		(__force level_t)((__force u8)(level) - 1) )

/* 
                                                  
  
                                               
                                                        
                                     
                                                          
                                       
                                                       
                               
                       
 */
struct logfs_area { /*                            */
	struct super_block *a_sb;
	int	a_is_open;
	u32	a_segno;
	u32	a_written_bytes;
	u32	a_used_bytes;
	const struct logfs_area_ops *a_ops;
	u32	a_erase_count;
	gc_level_t a_level;
};

/* 
                                          
  
                                                                       
                                                              
                                           
 */
struct logfs_area_ops {
	void	(*get_free_segment)(struct logfs_area *area);
	void	(*get_erase_count)(struct logfs_area *area);
	int	(*erase_segment)(struct logfs_area *area);
};

struct logfs_super;	/*         */
/* 
                                                     
  
                                       
                                                            
                              
                                
                                     
                                                             
 */
struct logfs_device_ops {
	struct page *(*find_first_sb)(struct super_block *sb, u64 *ofs);
	struct page *(*find_last_sb)(struct super_block *sb, u64 *ofs);
	int (*write_sb)(struct super_block *sb, struct page *page);
	int (*readpage)(void *_sb, struct page *page);
	void (*writeseg)(struct super_block *sb, u64 ofs, size_t len);
	int (*erase)(struct super_block *sb, loff_t ofs, size_t len,
			int ensure_write);
	int (*can_write_buf)(struct super_block *sb, u64 ofs);
	void (*sync)(struct super_block *sb);
	void (*put_device)(struct logfs_super *s);
};

/* 
                                                     
 */
struct candidate_list {
	struct rb_root rb_tree;
	int count;
	int maxcount;
	int sort_by_ec;
};

/* 
                                                                         
  
                                     
                           
                                  
                                        
                                   
  
                                                                          
                                                                         
                                                                       
                                                                         
                                                                       
 */
struct gc_candidate {
	struct rb_node rb_node;
	struct candidate_list *list;
	u32	segno;
	u32	valid;
	u32	erase_count;
	u8	dist;
};

/* 
                                                                            
  
         
                                 
                 
                    
 */
struct logfs_journal_entry {
	int used;
	s16 version;
	u16 len;
	u16 datalen;
	u64 offset;
};

enum transaction_state {
	CREATE_1 = 1,
	CREATE_2,
	UNLINK_1,
	UNLINK_2,
	CROSS_RENAME_1,
	CROSS_RENAME_2,
	TARGET_RENAME_1,
	TARGET_RENAME_2,
	TARGET_RENAME_3
};

/* 
                                                                       
  
                       
                                               
                                     
 */
struct logfs_transaction {
	enum transaction_state state;
	u64	 ino;
	u64	 dir;
	u64	 pos;
};

/* 
                                                                               
                                            
                                            
                       
                      
                                                  
                                                  
                        
 */
struct logfs_shadow {
	u64 old_ofs;
	u64 new_ofs;
	u64 ino;
	u64 bix;
	int old_len;
	int new_len;
	gc_level_t gc_level;
};

/* 
                     
                                                       
                                                       
                                                         
                                                              
 */
struct shadow_tree {
	struct btree_head64 new;
	struct btree_head64 old;
	struct btree_head32 segment_map;
	int no_shadowed_segments;
};

struct object_alias_item {
	struct list_head list;
	__be64 val;
	int child_no;
};

/* 
                                                
                                   
                                              
                                                     
  
                                                                              
                                                                            
                                                                            
                                                                          
                                           
 */
#define BLOCK_INDIRECT	1	/*                */
#define BLOCK_INODE	2	/*       */
struct logfs_block_ops;
struct logfs_block {
	struct list_head alias_list;
	struct list_head item_list;
	struct super_block *sb;
	u64 ino;
	u64 bix;
	level_t level;
	struct page *page;
	struct inode *inode;
	struct logfs_transaction *ta;
	unsigned long alias_map[LOGFS_BLOCK_FACTOR / BITS_PER_LONG];
	struct logfs_block_ops *ops;
	int full;
	int partial;
	int reserved_bytes;
};

typedef int write_alias_t(struct super_block *sb, u64 ino, u64 bix,
		level_t level, int child_no, __be64 val);
struct logfs_block_ops {
	void	(*write_block)(struct logfs_block *block);
	void	(*free_block)(struct super_block *sb, struct logfs_block*block);
	int	(*write_alias)(struct super_block *sb,
			struct logfs_block *block,
			write_alias_t *write_one_alias);
};

#define MAX_JOURNAL_ENTRIES 256

struct logfs_super {
	struct mtd_info *s_mtd;			/*                   */
	struct block_device *s_bdev;		/*                   */
	const struct logfs_device_ops *s_devops;/*               */
	struct inode	*s_master_inode;	/*            */
	struct inode	*s_segfile_inode;	/*              */
	struct inode *s_mapping_inode;		/*                */
	atomic_t s_pending_writes;		/*                  */
	long	 s_flags;
	mempool_t *s_btree_pool;		/*                 */
	mempool_t *s_alias_pool;		/*                      */
	u64	 s_feature_incompat;
	u64	 s_feature_ro_compat;
	u64	 s_feature_compat;
	u64	 s_feature_flags;
	u64	 s_sb_ofs[2];
	struct page *s_erase_page;		/*                */
	/*                */
	struct btree_head32 s_segment_alias;	/*                   */
	int	 s_no_object_aliases;
	struct list_head s_object_alias;	/*                  */
	struct btree_head128 s_object_alias_tree; /*                  */
	struct mutex s_object_alias_mutex;
	/*              */
	struct mutex s_dirop_mutex;		/*                         */
	u64	 s_victim_ino;			/*                         */
	u64	 s_rename_dir;			/*                      */
	u64	 s_rename_pos;			/*                       */
	/*             */
	long	 s_segsize;			/*                   */
	int	 s_segshift;			/*                      */
	long	 s_segmask;			/*                     */
	long	 s_no_segs;			/*                    */
	long	 s_no_journal_segs;		/*                           */
	long	 s_no_blocks;			/*                    */
	long	 s_writesize;			/*                    */
	int	 s_writeshift;			/*                    */
	u64	 s_size;			/*                 */
	struct logfs_area *s_area[LOGFS_NO_AREAS];	/*                    */
	u64	 s_gec;				/*                    */
	u64	 s_wl_gec_ostore;		/*                       */
	u64	 s_wl_gec_journal;		/*                       */
	u64	 s_sweeper;			/*                     */
	u8	 s_ifile_levels;		/*                    */
	u8	 s_iblock_levels;		/*                            */
	u8	 s_data_levels;			/*                            */
	u8	 s_total_levels;		/*                    */
	struct btree_head32 s_cand_tree;	/*                */
	struct candidate_list s_free_list;	/*                    */
	struct candidate_list s_reserve_list;	/*                     */
	struct candidate_list s_low_list[LOGFS_NO_AREAS];/*                 */
	struct candidate_list s_ec_list;	/*                       */
	struct btree_head32 s_reserved_segments;/*                        */
	/*                */
	u64	 s_last_ino;			/*                  */
	long	 s_inos_till_wrap;
	u32	 s_generation;			/*                            */
	struct list_head s_freeing_list;	/*                    */
	/*                  */
	struct mutex s_journal_mutex;
	void	*s_je;				/*                           */
	void	*s_compressed_je;		/*                           */
	u32	 s_journal_seg[LOGFS_JOURNAL_SEGS]; /*                  */
	u32	 s_journal_ec[LOGFS_JOURNAL_SEGS]; /*                     */
	u64	 s_last_version;
	struct logfs_area *s_journal_area;	/*                      */
	__be64	s_je_array[MAX_JOURNAL_ENTRIES];
	int	s_no_je;

	int	 s_sum_index;			/*                      */
	struct shadow_tree s_shadow_tree;
	int	 s_je_fill;			/*                     */
	/*                    */
	struct mutex s_write_mutex;
	int	 s_lock_count;
	mempool_t *s_block_pool;		/*                         */
	mempool_t *s_shadow_pool;		/*                          */
	struct list_head s_writeback_list;	/*                 */
	/*
                     
                                                                    
                                                                  
                                                                   
                                                                     
                                        
                                                                 
                                                                   
                                                                    
                              
                                                                
              
                                                                  
                                                                   
                                      
                                                               
                                     
                                                                    
                                                                  
                     
   
                                                                     
  */
	u64	 s_free_bytes;
	u64	 s_used_bytes;
	u64	 s_dirty_free_bytes;
	u64	 s_dirty_used_bytes;
	u64	 s_root_reserve;
	u64	 s_speed_reserve;
	u64	 s_dirty_pages;
	/*                    
                                                            
                                                                      
                                                   
                                                         
  */
	u32	 s_bad_seg_reserve;
	u32	 s_bad_segments;
};

/* 
                                       
  
                             
                            
                                        
                                                                
                           
                                                            
 */
struct logfs_inode {
	struct inode vfs_inode;
	u64	li_data[LOGFS_EMBEDDED_FIELDS];
	u64	li_used_bytes;
	struct list_head li_freeing_list;
	struct logfs_block *li_block;
	u32	li_flags;
	u8	li_height;
	int	li_refcount;
};

#define journal_for_each(__i) for (__i = 0; __i < LOGFS_JOURNAL_SEGS; __i++)
#define for_each_area(__i) for (__i = 0; __i < LOGFS_NO_AREAS; __i++)
#define for_each_area_down(__i) for (__i = LOGFS_NO_AREAS - 1; __i >= 0; __i--)

/*         */
int logfs_compress(void *in, void *out, size_t inlen, size_t outlen);
int logfs_uncompress(void *in, void *out, size_t inlen, size_t outlen);
int __init logfs_compr_init(void);
void logfs_compr_exit(void);

/*            */
#ifdef CONFIG_BLOCK
int logfs_get_sb_bdev(struct logfs_super *s,
		struct file_system_type *type,
		const char *devname);
#else
static inline int logfs_get_sb_bdev(struct logfs_super *s,
		struct file_system_type *type,
		const char *devname)
{
	return -ENODEV;
}
#endif

/*           */
#ifdef CONFIG_MTD
int logfs_get_sb_mtd(struct logfs_super *s, int mtdnr);
#else
static inline int logfs_get_sb_mtd(struct logfs_super *s, int mtdnr)
{
	return -ENODEV;
}
#endif

/*       */
extern const struct inode_operations logfs_symlink_iops;
extern const struct inode_operations logfs_dir_iops;
extern const struct file_operations logfs_dir_fops;
int logfs_replay_journal(struct super_block *sb);

/*        */
extern const struct inode_operations logfs_reg_iops;
extern const struct file_operations logfs_reg_fops;
extern const struct address_space_operations logfs_reg_aops;
int logfs_readpage(struct file *file, struct page *page);
long logfs_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
int logfs_fsync(struct file *file, loff_t start, loff_t end, int datasync);

/*      */
u32 get_best_cand(struct super_block *sb, struct candidate_list *list, u32 *ec);
void logfs_gc_pass(struct super_block *sb);
int logfs_check_areas(struct super_block *sb);
int logfs_init_gc(struct super_block *sb);
void logfs_cleanup_gc(struct super_block *sb);

/*         */
extern const struct super_operations logfs_super_operations;
struct inode *logfs_iget(struct super_block *sb, ino_t ino);
struct inode *logfs_safe_iget(struct super_block *sb, ino_t ino, int *cookie);
void logfs_safe_iput(struct inode *inode, int cookie);
struct inode *logfs_new_inode(struct inode *dir, umode_t mode);
struct inode *logfs_new_meta_inode(struct super_block *sb, u64 ino);
struct inode *logfs_read_meta_inode(struct super_block *sb, u64 ino);
int logfs_init_inode_cache(void);
void logfs_destroy_inode_cache(void);
void logfs_set_blocks(struct inode *inode, u64 no);
/*                                                                        */
int logfs_read_inode(struct inode *inode);
int __logfs_write_inode(struct inode *inode, struct page *, long flags);
void logfs_evict_inode(struct inode *inode);

/*           */
void logfs_write_anchor(struct super_block *sb);
int logfs_init_journal(struct super_block *sb);
void logfs_cleanup_journal(struct super_block *sb);
int write_alias_journal(struct super_block *sb, u64 ino, u64 bix,
		level_t level, int child_no, __be64 val);
void do_logfs_journal_wl_pass(struct super_block *sb);

/*             */
pgoff_t logfs_pack_index(u64 bix, level_t level);
void logfs_unpack_index(pgoff_t index, u64 *bix, level_t *level);
int logfs_inode_write(struct inode *inode, const void *buf, size_t count,
		loff_t bix, long flags, struct shadow_tree *shadow_tree);
int logfs_readpage_nolock(struct page *page);
int logfs_write_buf(struct inode *inode, struct page *page, long flags);
int logfs_delete(struct inode *inode, pgoff_t index,
		struct shadow_tree *shadow_tree);
int logfs_rewrite_block(struct inode *inode, u64 bix, u64 ofs,
		gc_level_t gc_level, long flags);
int logfs_is_valid_block(struct super_block *sb, u64 ofs, u64 ino, u64 bix,
		gc_level_t gc_level);
int logfs_truncate(struct inode *inode, u64 size);
u64 logfs_seek_hole(struct inode *inode, u64 bix);
u64 logfs_seek_data(struct inode *inode, u64 bix);
int logfs_open_segfile(struct super_block *sb);
int logfs_init_rw(struct super_block *sb);
void logfs_cleanup_rw(struct super_block *sb);
void logfs_add_transaction(struct inode *inode, struct logfs_transaction *ta);
void logfs_del_transaction(struct inode *inode, struct logfs_transaction *ta);
void logfs_write_block(struct logfs_block *block, long flags);
int logfs_write_obj_aliases_pagecache(struct super_block *sb);
void logfs_get_segment_entry(struct super_block *sb, u32 segno,
		struct logfs_segment_entry *se);
void logfs_set_segment_used(struct super_block *sb, u64 ofs, int increment);
void logfs_set_segment_erased(struct super_block *sb, u32 segno, u32 ec,
		gc_level_t gc_level);
void logfs_set_segment_reserved(struct super_block *sb, u32 segno);
void logfs_set_segment_unreserved(struct super_block *sb, u32 segno, u32 ec);
struct logfs_block *__alloc_block(struct super_block *sb,
		u64 ino, u64 bix, level_t level);
void __free_block(struct super_block *sb, struct logfs_block *block);
void btree_write_block(struct logfs_block *block);
void initialize_block_counters(struct page *page, struct logfs_block *block,
		__be64 *array, int page_is_empty);
int logfs_exist_block(struct inode *inode, u64 bix);
int get_page_reserve(struct inode *inode, struct page *page);
void logfs_get_wblocks(struct super_block *sb, struct page *page, int lock);
void logfs_put_wblocks(struct super_block *sb, struct page *page, int lock);
extern struct logfs_block_ops indirect_block_ops;

/*           */
int logfs_erase_segment(struct super_block *sb, u32 ofs, int ensure_erase);
int wbuf_read(struct super_block *sb, u64 ofs, size_t len, void *buf);
int logfs_segment_read(struct inode *inode, struct page *page, u64 ofs, u64 bix,
		level_t level);
int logfs_segment_write(struct inode *inode, struct page *page,
		struct logfs_shadow *shadow);
int logfs_segment_delete(struct inode *inode, struct logfs_shadow *shadow);
int logfs_load_object_aliases(struct super_block *sb,
		struct logfs_obj_alias *oa, int count);
void move_page_to_btree(struct page *page);
int logfs_init_mapping(struct super_block *sb);
void logfs_sync_area(struct logfs_area *area);
void logfs_sync_segments(struct super_block *sb);
void freeseg(struct super_block *sb, u32 segno);
void free_areas(struct super_block *sb);

/*               */
int logfs_init_areas(struct super_block *sb);
void logfs_cleanup_areas(struct super_block *sb);
int logfs_open_area(struct logfs_area *area, size_t bytes);
int __logfs_buf_write(struct logfs_area *area, u64 ofs, void *buf, size_t len,
		int use_filler);

static inline int logfs_buf_write(struct logfs_area *area, u64 ofs,
		void *buf, size_t len)
{
	return __logfs_buf_write(area, ofs, buf, len, 0);
}

static inline int logfs_buf_recover(struct logfs_area *area, u64 ofs,
		void *buf, size_t len)
{
	return __logfs_buf_write(area, ofs, buf, len, 1);
}

/*         */
struct page *emergency_read_begin(struct address_space *mapping, pgoff_t index);
void emergency_read_end(struct page *page);
void logfs_crash_dump(struct super_block *sb);
int logfs_statfs(struct dentry *dentry, struct kstatfs *stats);
int logfs_check_ds(struct logfs_disk_super *ds);
int logfs_write_sb(struct super_block *sb);

static inline struct logfs_super *logfs_super(struct super_block *sb)
{
	return sb->s_fs_info;
}

static inline struct logfs_inode *logfs_inode(struct inode *inode)
{
	return container_of(inode, struct logfs_inode, vfs_inode);
}

static inline void logfs_set_ro(struct super_block *sb)
{
	logfs_super(sb)->s_flags |= LOGFS_SB_FLAG_RO;
}

#define LOGFS_BUG(sb) do {					\
	struct super_block *__sb = sb;				\
	logfs_crash_dump(__sb);					\
	logfs_super(__sb)->s_flags |= LOGFS_SB_FLAG_RO;		\
	BUG();							\
} while (0)

#define LOGFS_BUG_ON(condition, sb) \
	do { if (unlikely(condition)) LOGFS_BUG((sb)); } while (0)

static inline __be32 logfs_crc32(void *data, size_t len, size_t skip)
{
	return cpu_to_be32(crc32(~0, data+skip, len-skip));
}

static inline u8 logfs_type(struct inode *inode)
{
	return (inode->i_mode >> 12) & 15;
}

static inline pgoff_t logfs_index(struct super_block *sb, u64 pos)
{
	return pos >> sb->s_blocksize_bits;
}

static inline u64 dev_ofs(struct super_block *sb, u32 segno, u32 ofs)
{
	return ((u64)segno << logfs_super(sb)->s_segshift) + ofs;
}

static inline u32 seg_no(struct super_block *sb, u64 ofs)
{
	return ofs >> logfs_super(sb)->s_segshift;
}

static inline u32 seg_ofs(struct super_block *sb, u64 ofs)
{
	return ofs & logfs_super(sb)->s_segmask;
}

static inline u64 seg_align(struct super_block *sb, u64 ofs)
{
	return ofs & ~logfs_super(sb)->s_segmask;
}

static inline struct logfs_block *logfs_block(struct page *page)
{
	return (void *)page->private;
}

static inline level_t shrink_level(gc_level_t __level)
{
	u8 level = (__force u8)__level;

	if (level >= LOGFS_MAX_LEVELS)
		level -= LOGFS_MAX_LEVELS;
	return (__force level_t)level;
}

static inline gc_level_t expand_level(u64 ino, level_t __level)
{
	u8 level = (__force u8)__level;

	if (ino == LOGFS_INO_MASTER) {
		/*                          */
		level += LOGFS_MAX_LEVELS;
	}
	return (__force gc_level_t)level;
}

static inline int logfs_block_shift(struct super_block *sb, level_t level)
{
	level = shrink_level((__force gc_level_t)level);
	return (__force int)level * (sb->s_blocksize_bits - 3);
}

static inline u64 logfs_block_mask(struct super_block *sb, level_t level)
{
	return ~0ull << logfs_block_shift(sb, level);
}

static inline struct logfs_area *get_area(struct super_block *sb,
		gc_level_t gc_level)
{
	return logfs_super(sb)->s_area[(__force u8)gc_level];
}

static inline void logfs_mempool_destroy(mempool_t *pool)
{
	if (pool)
		mempool_destroy(pool);
}

#endif
