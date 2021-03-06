/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
 */

/*
                                                 
                                            
                                                     
  
  
                                                                             
                                                                      
                                                                            
                                                                         
                                                                         
                                                                 
                                                                             
                                                                          
                                                                            
  
                                                                            
                                                                              
                                                                              
                                                                            
                                                                           
                                                                           
                                                           
  
                                                                              
                                                                               
                                                                                
                                                                 
  
                                             
  
                                                                             
  
                                                                                
                                                                             
                                                                           
                                                                           
                                                                          
                          
  
                                                                    
                                                                            
                                                                                
                                                                 
 */

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/buffer_head.h>
#include <linux/sort.h>
#include <linux/gfs2_ondisk.h>
#include <linux/crc32.h>
#include <linux/vmalloc.h>

#include "gfs2.h"
#include "incore.h"
#include "dir.h"
#include "glock.h"
#include "inode.h"
#include "meta_io.h"
#include "quota.h"
#include "rgrp.h"
#include "trans.h"
#include "bmap.h"
#include "util.h"

#define IS_LEAF     1 /*                         */
#define IS_DINODE   2 /*                                         */

#define MAX_RA_BLOCKS 32 /*                       */

#define gfs2_disk_hash2offset(h) (((u64)(h)) >> 1)
#define gfs2_dir_offset2hash(p) ((u32)(((u64)(p)) << 1))

struct qstr gfs2_qdot __read_mostly;
struct qstr gfs2_qdotdot __read_mostly;

typedef int (*gfs2_dscan_t)(const struct gfs2_dirent *dent,
			    const struct qstr *name, void *opaque);

int gfs2_dir_get_new_buffer(struct gfs2_inode *ip, u64 block,
			    struct buffer_head **bhp)
{
	struct buffer_head *bh;

	bh = gfs2_meta_new(ip->i_gl, block);
	gfs2_trans_add_bh(ip->i_gl, bh, 1);
	gfs2_metatype_set(bh, GFS2_METATYPE_JD, GFS2_FORMAT_JD);
	gfs2_buffer_clear_tail(bh, sizeof(struct gfs2_meta_header));
	*bhp = bh;
	return 0;
}

static int gfs2_dir_get_existing_buffer(struct gfs2_inode *ip, u64 block,
					struct buffer_head **bhp)
{
	struct buffer_head *bh;
	int error;

	error = gfs2_meta_read(ip->i_gl, block, DIO_WAIT, &bh);
	if (error)
		return error;
	if (gfs2_metatype_check(GFS2_SB(&ip->i_inode), bh, GFS2_METATYPE_JD)) {
		brelse(bh);
		return -EIO;
	}
	*bhp = bh;
	return 0;
}

static int gfs2_dir_write_stuffed(struct gfs2_inode *ip, const char *buf,
				  unsigned int offset, unsigned int size)
{
	struct buffer_head *dibh;
	int error;

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error)
		return error;

	gfs2_trans_add_bh(ip->i_gl, dibh, 1);
	memcpy(dibh->b_data + offset + sizeof(struct gfs2_dinode), buf, size);
	if (ip->i_inode.i_size < offset + size)
		i_size_write(&ip->i_inode, offset + size);
	ip->i_inode.i_mtime = ip->i_inode.i_ctime = CURRENT_TIME;
	gfs2_dinode_out(ip, dibh->b_data);

	brelse(dibh);

	return size;
}



/* 
                                                                 
                      
                                                        
                                               
                                     
  
                                                               
 */
static int gfs2_dir_write_data(struct gfs2_inode *ip, const char *buf,
			       u64 offset, unsigned int size)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	struct buffer_head *dibh;
	u64 lblock, dblock;
	u32 extlen = 0;
	unsigned int o;
	int copied = 0;
	int error = 0;
	int new = 0;

	if (!size)
		return 0;

	if (gfs2_is_stuffed(ip) &&
	    offset + size <= sdp->sd_sb.sb_bsize - sizeof(struct gfs2_dinode))
		return gfs2_dir_write_stuffed(ip, buf, (unsigned int)offset,
					      size);

	if (gfs2_assert_warn(sdp, gfs2_is_jdata(ip)))
		return -EINVAL;

	if (gfs2_is_stuffed(ip)) {
		error = gfs2_unstuff_dinode(ip, NULL);
		if (error)
			return error;
	}

	lblock = offset;
	o = do_div(lblock, sdp->sd_jbsize) + sizeof(struct gfs2_meta_header);

	while (copied < size) {
		unsigned int amount;
		struct buffer_head *bh;

		amount = size - copied;
		if (amount > sdp->sd_sb.sb_bsize - o)
			amount = sdp->sd_sb.sb_bsize - o;

		if (!extlen) {
			new = 1;
			error = gfs2_extent_map(&ip->i_inode, lblock, &new,
						&dblock, &extlen);
			if (error)
				goto fail;
			error = -EIO;
			if (gfs2_assert_withdraw(sdp, dblock))
				goto fail;
		}

		if (amount == sdp->sd_jbsize || new)
			error = gfs2_dir_get_new_buffer(ip, dblock, &bh);
		else
			error = gfs2_dir_get_existing_buffer(ip, dblock, &bh);

		if (error)
			goto fail;

		gfs2_trans_add_bh(ip->i_gl, bh, 1);
		memcpy(bh->b_data + o, buf, amount);
		brelse(bh);

		buf += amount;
		copied += amount;
		lblock++;
		dblock++;
		extlen--;

		o = sizeof(struct gfs2_meta_header);
	}

out:
	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error)
		return error;

	if (ip->i_inode.i_size < offset + copied)
		i_size_write(&ip->i_inode, offset + copied);
	ip->i_inode.i_mtime = ip->i_inode.i_ctime = CURRENT_TIME;

	gfs2_trans_add_bh(ip->i_gl, dibh, 1);
	gfs2_dinode_out(ip, dibh->b_data);
	brelse(dibh);

	return copied;
fail:
	if (copied)
		goto out;
	return error;
}

static int gfs2_dir_read_stuffed(struct gfs2_inode *ip, __be64 *buf,
				 unsigned int size)
{
	struct buffer_head *dibh;
	int error;

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (!error) {
		memcpy(buf, dibh->b_data + sizeof(struct gfs2_dinode), size);
		brelse(dibh);
	}

	return (error) ? error : size;
}


/* 
                                                          
                      
                                        
                                    
  
                                                           
 */
static int gfs2_dir_read_data(struct gfs2_inode *ip, __be64 *buf,
			      unsigned int size)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	u64 lblock, dblock;
	u32 extlen = 0;
	unsigned int o;
	int copied = 0;
	int error = 0;

	if (gfs2_is_stuffed(ip))
		return gfs2_dir_read_stuffed(ip, buf, size);

	if (gfs2_assert_warn(sdp, gfs2_is_jdata(ip)))
		return -EINVAL;

	lblock = 0;
	o = do_div(lblock, sdp->sd_jbsize) + sizeof(struct gfs2_meta_header);

	while (copied < size) {
		unsigned int amount;
		struct buffer_head *bh;
		int new;

		amount = size - copied;
		if (amount > sdp->sd_sb.sb_bsize - o)
			amount = sdp->sd_sb.sb_bsize - o;

		if (!extlen) {
			new = 0;
			error = gfs2_extent_map(&ip->i_inode, lblock, &new,
						&dblock, &extlen);
			if (error || !dblock)
				goto fail;
			BUG_ON(extlen < 1);
			bh = gfs2_meta_ra(ip->i_gl, dblock, extlen);
		} else {
			error = gfs2_meta_read(ip->i_gl, dblock, DIO_WAIT, &bh);
			if (error)
				goto fail;
		}
		error = gfs2_metatype_check(sdp, bh, GFS2_METATYPE_JD);
		if (error) {
			brelse(bh);
			goto fail;
		}
		dblock++;
		extlen--;
		memcpy(buf, bh->b_data + o, amount);
		brelse(bh);
		buf += (amount/sizeof(__be64));
		copied += amount;
		lblock++;
		o = sizeof(struct gfs2_meta_header);
	}

	return copied;
fail:
	return (copied) ? copied : error;
}

/* 
                                                              
                             
  
                                      
 */

static __be64 *gfs2_dir_get_hash_table(struct gfs2_inode *ip)
{
	struct inode *inode = &ip->i_inode;
	int ret;
	u32 hsize;
	__be64 *hc;

	BUG_ON(!(ip->i_diskflags & GFS2_DIF_EXHASH));

	hc = ip->i_hash_cache;
	if (hc)
		return hc;

	hsize = 1 << ip->i_depth;
	hsize *= sizeof(__be64);
	if (hsize != i_size_read(&ip->i_inode)) {
		gfs2_consist_inode(ip);
		return ERR_PTR(-EIO);
	}

	hc = kmalloc(hsize, GFP_NOFS);
	ret = -ENOMEM;
	if (hc == NULL)
		return ERR_PTR(-ENOMEM);

	ret = gfs2_dir_read_data(ip, hc, hsize);
	if (ret < 0) {
		kfree(hc);
		return ERR_PTR(ret);
	}

	spin_lock(&inode->i_lock);
	if (ip->i_hash_cache)
		kfree(hc);
	else
		ip->i_hash_cache = hc;
	spin_unlock(&inode->i_lock);

	return ip->i_hash_cache;
}

/* 
                                            
                           
  
                                                                        
 */
void gfs2_dir_hash_inval(struct gfs2_inode *ip)
{
	__be64 *hc = ip->i_hash_cache;
	ip->i_hash_cache = NULL;
	kfree(hc);
}

static inline int gfs2_dirent_sentinel(const struct gfs2_dirent *dent)
{
	return dent->de_inum.no_addr == 0 || dent->de_inum.no_formal_ino == 0;
}

static inline int __gfs2_dirent_find(const struct gfs2_dirent *dent,
				     const struct qstr *name, int ret)
{
	if (!gfs2_dirent_sentinel(dent) &&
	    be32_to_cpu(dent->de_hash) == name->hash &&
	    be16_to_cpu(dent->de_name_len) == name->len &&
	    memcmp(dent+1, name->name, name->len) == 0)
		return ret;
	return 0;
}

static int gfs2_dirent_find(const struct gfs2_dirent *dent,
			    const struct qstr *name,
			    void *opaque)
{
	return __gfs2_dirent_find(dent, name, 1);
}

static int gfs2_dirent_prev(const struct gfs2_dirent *dent,
			    const struct qstr *name,
			    void *opaque)
{
	return __gfs2_dirent_find(dent, name, 2);
}

/*
                                          
                                 
 */
static int gfs2_dirent_last(const struct gfs2_dirent *dent,
			    const struct qstr *name,
			    void *opaque)
{
	const char *start = name->name;
	const char *end = (const char *)dent + be16_to_cpu(dent->de_rec_len);
	if (name->len == (end - start))
		return 1;
	return 0;
}

static int gfs2_dirent_find_space(const struct gfs2_dirent *dent,
				  const struct qstr *name,
				  void *opaque)
{
	unsigned required = GFS2_DIRENT_SIZE(name->len);
	unsigned actual = GFS2_DIRENT_SIZE(be16_to_cpu(dent->de_name_len));
	unsigned totlen = be16_to_cpu(dent->de_rec_len);

	if (gfs2_dirent_sentinel(dent))
		actual = 0;
	if (totlen - actual >= required)
		return 1;
	return 0;
}

struct dirent_gather {
	const struct gfs2_dirent **pdent;
	unsigned offset;
};

static int gfs2_dirent_gather(const struct gfs2_dirent *dent,
			      const struct qstr *name,
			      void *opaque)
{
	struct dirent_gather *g = opaque;
	if (!gfs2_dirent_sentinel(dent)) {
		g->pdent[g->offset++] = dent;
	}
	return 0;
}

/*
                                  
                                                              
                               
                                                                     
                                                                        
                                                                      
               
 */
static int gfs2_check_dirent(struct gfs2_dirent *dent, unsigned int offset,
			     unsigned int size, unsigned int len, int first)
{
	const char *msg = "gfs2_dirent too small";
	if (unlikely(size < sizeof(struct gfs2_dirent)))
		goto error;
	msg = "gfs2_dirent misaligned";
	if (unlikely(offset & 0x7))
		goto error;
	msg = "gfs2_dirent points beyond end of block";
	if (unlikely(offset + size > len))
		goto error;
	msg = "zero inode number";
	if (unlikely(!first && gfs2_dirent_sentinel(dent)))
		goto error;
	msg = "name length is greater than space in dirent";
	if (!gfs2_dirent_sentinel(dent) &&
	    unlikely(sizeof(struct gfs2_dirent)+be16_to_cpu(dent->de_name_len) >
		     size))
		goto error;
	return 0;
error:
	printk(KERN_WARNING "gfs2_check_dirent: %s (%s)\n", msg,
	       first ? "first in block" : "not first in block");
	return -EIO;
}

static int gfs2_dirent_offset(const void *buf)
{
	const struct gfs2_meta_header *h = buf;
	int offset;

	BUG_ON(buf == NULL);

	switch(be32_to_cpu(h->mh_type)) {
	case GFS2_METATYPE_LF:
		offset = sizeof(struct gfs2_leaf);
		break;
	case GFS2_METATYPE_DI:
		offset = sizeof(struct gfs2_dinode);
		break;
	default:
		goto wrong_type;
	}
	return offset;
wrong_type:
	printk(KERN_WARNING "gfs2_scan_dirent: wrong block type %u\n",
	       be32_to_cpu(h->mh_type));
	return -1;
}

static struct gfs2_dirent *gfs2_dirent_scan(struct inode *inode, void *buf,
					    unsigned int len, gfs2_dscan_t scan,
					    const struct qstr *name,
					    void *opaque)
{
	struct gfs2_dirent *dent, *prev;
	unsigned offset;
	unsigned size;
	int ret = 0;

	ret = gfs2_dirent_offset(buf);
	if (ret < 0)
		goto consist_inode;

	offset = ret;
	prev = NULL;
	dent = buf + offset;
	size = be16_to_cpu(dent->de_rec_len);
	if (gfs2_check_dirent(dent, offset, size, len, 1))
		goto consist_inode;
	do {
		ret = scan(dent, name, opaque);
		if (ret)
			break;
		offset += size;
		if (offset == len)
			break;
		prev = dent;
		dent = buf + offset;
		size = be16_to_cpu(dent->de_rec_len);
		if (gfs2_check_dirent(dent, offset, size, len, 0))
			goto consist_inode;
	} while(1);

	switch(ret) {
	case 0:
		return NULL;
	case 1:
		return dent;
	case 2:
		return prev ? prev : dent;
	default:
		BUG_ON(ret > 0);
		return ERR_PTR(ret);
	}

consist_inode:
	gfs2_consist_inode(GFS2_I(inode));
	return ERR_PTR(-EIO);
}

static int dirent_check_reclen(struct gfs2_inode *dip,
			       const struct gfs2_dirent *d, const void *end_p)
{
	const void *ptr = d;
	u16 rec_len = be16_to_cpu(d->de_rec_len);

	if (unlikely(rec_len < sizeof(struct gfs2_dirent)))
		goto broken;
	ptr += rec_len;
	if (ptr < end_p)
		return rec_len;
	if (ptr == end_p)
		return -ENOENT;
broken:
	gfs2_consist_inode(dip);
	return -EIO;
}

/* 
                            
                      
                  
                                    
  
                                              
 */

static int dirent_next(struct gfs2_inode *dip, struct buffer_head *bh,
		       struct gfs2_dirent **dent)
{
	struct gfs2_dirent *cur = *dent, *tmp;
	char *bh_end = bh->b_data + bh->b_size;
	int ret;

	ret = dirent_check_reclen(dip, cur, bh_end);
	if (ret < 0)
		return ret;

	tmp = (void *)cur + ret;
	ret = dirent_check_reclen(dip, tmp, bh_end);
	if (ret == -EIO)
		return ret;

        /*                                                          */
	if (gfs2_dirent_sentinel(tmp)) {
		gfs2_consist_inode(dip);
		return -EIO;
	}

	*dent = tmp;
	return 0;
}

/* 
                               
                       
                  
                             
                           
  
 */

static void dirent_del(struct gfs2_inode *dip, struct buffer_head *bh,
		       struct gfs2_dirent *prev, struct gfs2_dirent *cur)
{
	u16 cur_rec_len, prev_rec_len;

	if (gfs2_dirent_sentinel(cur)) {
		gfs2_consist_inode(dip);
		return;
	}

	gfs2_trans_add_bh(dip->i_gl, bh, 1);

	/*                                                                 
                                                                  
                                      */

	if (!prev) {
		cur->de_inum.no_addr = 0;
		cur->de_inum.no_formal_ino = 0;
		return;
	}

	/*                                              */

	prev_rec_len = be16_to_cpu(prev->de_rec_len);
	cur_rec_len = be16_to_cpu(cur->de_rec_len);

	if ((char *)prev + prev_rec_len != (char *)cur)
		gfs2_consist_inode(dip);
	if ((char *)cur + cur_rec_len > bh->b_data + bh->b_size)
		gfs2_consist_inode(dip);

	prev_rec_len += cur_rec_len;
	prev->de_rec_len = cpu_to_be16(prev_rec_len);
}

/*
                                                                    
                      
 */
static struct gfs2_dirent *gfs2_init_dirent(struct inode *inode,
					    struct gfs2_dirent *dent,
					    const struct qstr *name,
					    struct buffer_head *bh)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_dirent *ndent;
	unsigned offset = 0, totlen;

	if (!gfs2_dirent_sentinel(dent))
		offset = GFS2_DIRENT_SIZE(be16_to_cpu(dent->de_name_len));
	totlen = be16_to_cpu(dent->de_rec_len);
	BUG_ON(offset + name->len > totlen);
	gfs2_trans_add_bh(ip->i_gl, bh, 1);
	ndent = (struct gfs2_dirent *)((char *)dent + offset);
	dent->de_rec_len = cpu_to_be16(offset);
	gfs2_qstr2dirent(name, totlen - offset, ndent);
	return ndent;
}

static struct gfs2_dirent *gfs2_dirent_alloc(struct inode *inode,
					     struct buffer_head *bh,
					     const struct qstr *name)
{
	struct gfs2_dirent *dent;
	dent = gfs2_dirent_scan(inode, bh->b_data, bh->b_size,
				gfs2_dirent_find_space, name, NULL);
	if (!dent || IS_ERR(dent))
		return dent;
	return gfs2_init_dirent(inode, dent, name, bh);
}

static int get_leaf(struct gfs2_inode *dip, u64 leaf_no,
		    struct buffer_head **bhp)
{
	int error;

	error = gfs2_meta_read(dip->i_gl, leaf_no, DIO_WAIT, bhp);
	if (!error && gfs2_metatype_check(GFS2_SB(&dip->i_inode), *bhp, GFS2_METATYPE_LF)) {
		/*                                                */
		error = -EIO;
	}

	return error;
}

/* 
                                                            
                       
          
             
  
                                              
 */

static int get_leaf_nr(struct gfs2_inode *dip, u32 index,
		       u64 *leaf_out)
{
	__be64 *hash;

	hash = gfs2_dir_get_hash_table(dip);
	if (IS_ERR(hash))
		return PTR_ERR(hash);
	*leaf_out = be64_to_cpu(*(hash + index));
	return 0;
}

static int get_first_leaf(struct gfs2_inode *dip, u32 index,
			  struct buffer_head **bh_out)
{
	u64 leaf_no;
	int error;

	error = get_leaf_nr(dip, index, &leaf_no);
	if (!error)
		error = get_leaf(dip, leaf_no, bh_out);

	return error;
}

static struct gfs2_dirent *gfs2_dirent_search(struct inode *inode,
					      const struct qstr *name,
					      gfs2_dscan_t scan,
					      struct buffer_head **pbh)
{
	struct buffer_head *bh;
	struct gfs2_dirent *dent;
	struct gfs2_inode *ip = GFS2_I(inode);
	int error;

	if (ip->i_diskflags & GFS2_DIF_EXHASH) {
		struct gfs2_leaf *leaf;
		unsigned hsize = 1 << ip->i_depth;
		unsigned index;
		u64 ln;
		if (hsize * sizeof(u64) != i_size_read(inode)) {
			gfs2_consist_inode(ip);
			return ERR_PTR(-EIO);
		}

		index = name->hash >> (32 - ip->i_depth);
		error = get_first_leaf(ip, index, &bh);
		if (error)
			return ERR_PTR(error);
		do {
			dent = gfs2_dirent_scan(inode, bh->b_data, bh->b_size,
						scan, name, NULL);
			if (dent)
				goto got_dent;
			leaf = (struct gfs2_leaf *)bh->b_data;
			ln = be64_to_cpu(leaf->lf_next);
			brelse(bh);
			if (!ln)
				break;

			error = get_leaf(ip, ln, &bh);
		} while(!error);

		return error ? ERR_PTR(error) : NULL;
	}


	error = gfs2_meta_inode_buffer(ip, &bh);
	if (error)
		return ERR_PTR(error);
	dent = gfs2_dirent_scan(inode, bh->b_data, bh->b_size, scan, name, NULL);
got_dent:
	if (unlikely(dent == NULL || IS_ERR(dent))) {
		brelse(bh);
		bh = NULL;
	}
	*pbh = bh;
	return dent;
}

static struct gfs2_leaf *new_leaf(struct inode *inode, struct buffer_head **pbh, u16 depth)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	unsigned int n = 1;
	u64 bn;
	int error;
	struct buffer_head *bh;
	struct gfs2_leaf *leaf;
	struct gfs2_dirent *dent;
	struct qstr name = { .name = "", .len = 0, .hash = 0 };

	error = gfs2_alloc_blocks(ip, &bn, &n, 0, NULL);
	if (error)
		return NULL;
	bh = gfs2_meta_new(ip->i_gl, bn);
	if (!bh)
		return NULL;

	gfs2_trans_add_unrevoke(GFS2_SB(inode), bn, 1);
	gfs2_trans_add_bh(ip->i_gl, bh, 1);
	gfs2_metatype_set(bh, GFS2_METATYPE_LF, GFS2_FORMAT_LF);
	leaf = (struct gfs2_leaf *)bh->b_data;
	leaf->lf_depth = cpu_to_be16(depth);
	leaf->lf_entries = 0;
	leaf->lf_dirent_format = cpu_to_be32(GFS2_FORMAT_DE);
	leaf->lf_next = 0;
	memset(leaf->lf_reserved, 0, sizeof(leaf->lf_reserved));
	dent = (struct gfs2_dirent *)(leaf+1);
	gfs2_qstr2dirent(&name, bh->b_size - sizeof(struct gfs2_leaf), dent);
	*pbh = bh;
	return leaf;
}

/* 
                                                                         
                       
  
                                              
 */

static int dir_make_exhash(struct inode *inode)
{
	struct gfs2_inode *dip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct gfs2_dirent *dent;
	struct qstr args;
	struct buffer_head *bh, *dibh;
	struct gfs2_leaf *leaf;
	int y;
	u32 x;
	__be64 *lp;
	u64 bn;
	int error;

	error = gfs2_meta_inode_buffer(dip, &dibh);
	if (error)
		return error;

	/*                        */

	leaf = new_leaf(inode, &bh, 0);
	if (!leaf)
		return -ENOSPC;
	bn = bh->b_blocknr;

	gfs2_assert(sdp, dip->i_entries < (1 << 16));
	leaf->lf_entries = cpu_to_be16(dip->i_entries);

	/*                */

	gfs2_buffer_copy_tail(bh, sizeof(struct gfs2_leaf), dibh,
			     sizeof(struct gfs2_dinode));

	/*                   */

	x = 0;
	args.len = bh->b_size - sizeof(struct gfs2_dinode) +
		   sizeof(struct gfs2_leaf);
	args.name = bh->b_data;
	dent = gfs2_dirent_scan(&dip->i_inode, bh->b_data, bh->b_size,
				gfs2_dirent_last, &args, NULL);
	if (!dent) {
		brelse(bh);
		brelse(dibh);
		return -EIO;
	}
	if (IS_ERR(dent)) {
		brelse(bh);
		brelse(dibh);
		return PTR_ERR(dent);
	}

	/*                                        
                                                          */

	dent->de_rec_len = cpu_to_be16(be16_to_cpu(dent->de_rec_len) +
		sizeof(struct gfs2_dinode) -
		sizeof(struct gfs2_leaf));

	brelse(bh);

	/*                                                       
                  */

	gfs2_trans_add_bh(dip->i_gl, dibh, 1);
	gfs2_buffer_clear_tail(dibh, sizeof(struct gfs2_dinode));

	lp = (__be64 *)(dibh->b_data + sizeof(struct gfs2_dinode));

	for (x = sdp->sd_hash_ptrs; x--; lp++)
		*lp = cpu_to_be64(bn);

	i_size_write(inode, sdp->sd_sb.sb_bsize / 2);
	gfs2_add_inode_blocks(&dip->i_inode, 1);
	dip->i_diskflags |= GFS2_DIF_EXHASH;

	for (x = sdp->sd_hash_ptrs, y = -1; x; x >>= 1, y++) ;
	dip->i_depth = y;

	gfs2_dinode_out(dip, dibh->b_data);

	brelse(dibh);

	return 0;
}

/* 
                                               
                       
          
            
  
                                               
 */

static int dir_split_leaf(struct inode *inode, const struct qstr *name)
{
	struct gfs2_inode *dip = GFS2_I(inode);
	struct buffer_head *nbh, *obh, *dibh;
	struct gfs2_leaf *nleaf, *oleaf;
	struct gfs2_dirent *dent = NULL, *prev = NULL, *next = NULL, *new;
	u32 start, len, half_len, divider;
	u64 bn, leaf_no;
	__be64 *lp;
	u32 index;
	int x, moved = 0;
	int error;

	index = name->hash >> (32 - dip->i_depth);
	error = get_leaf_nr(dip, index, &leaf_no);
	if (error)
		return error;

	/*                          */
	error = get_leaf(dip, leaf_no, &obh);
	if (error)
		return error;

	oleaf = (struct gfs2_leaf *)obh->b_data;
	if (dip->i_depth == be16_to_cpu(oleaf->lf_depth)) {
		brelse(obh);
		return 1; /*             */
	}

	gfs2_trans_add_bh(dip->i_gl, obh, 1);

	nleaf = new_leaf(inode, &nbh, be16_to_cpu(oleaf->lf_depth) + 1);
	if (!nleaf) {
		brelse(obh);
		return -ENOSPC;
	}
	bn = nbh->b_blocknr;

	/*                                                                 */
	len = 1 << (dip->i_depth - be16_to_cpu(oleaf->lf_depth));
	half_len = len >> 1;
	if (!half_len) {
		printk(KERN_WARNING "i_depth %u lf_depth %u index %u\n", dip->i_depth, be16_to_cpu(oleaf->lf_depth), index);
		gfs2_consist_inode(dip);
		error = -EIO;
		goto fail_brelse;
	}

	start = (index & ~(len - 1));

	/*                     
                                                         
                                             */
	lp = kmalloc(half_len * sizeof(__be64), GFP_NOFS);
	if (!lp) {
		error = -ENOMEM;
		goto fail_brelse;
	}

	/*                       */
	for (x = 0; x < half_len; x++)
		lp[x] = cpu_to_be64(bn);

	gfs2_dir_hash_inval(dip);

	error = gfs2_dir_write_data(dip, (char *)lp, start * sizeof(u64),
				    half_len * sizeof(u64));
	if (error != half_len * sizeof(u64)) {
		if (error >= 0)
			error = -EIO;
		goto fail_lpfree;
	}

	kfree(lp);

	/*                       */
	divider = (start + half_len) << (32 - dip->i_depth);

	/*                    */
	dent = (struct gfs2_dirent *)(obh->b_data + sizeof(struct gfs2_leaf));

	do {
		next = dent;
		if (dirent_next(dip, obh, &next))
			next = NULL;

		if (!gfs2_dirent_sentinel(dent) &&
		    be32_to_cpu(dent->de_hash) < divider) {
			struct qstr str;
			str.name = (char*)(dent+1);
			str.len = be16_to_cpu(dent->de_name_len);
			str.hash = be32_to_cpu(dent->de_hash);
			new = gfs2_dirent_alloc(inode, nbh, &str);
			if (IS_ERR(new)) {
				error = PTR_ERR(new);
				break;
			}

			new->de_inum = dent->de_inum; /*                   */
			new->de_type = dent->de_type; /*                   */
			be16_add_cpu(&nleaf->lf_entries, 1);

			dirent_del(dip, obh, prev, dent);

			if (!oleaf->lf_entries)
				gfs2_consist_inode(dip);
			be16_add_cpu(&oleaf->lf_entries, -1);

			if (!prev)
				prev = dent;

			moved = 1;
		} else {
			prev = dent;
		}
		dent = next;
	} while (dent);

	oleaf->lf_depth = nleaf->lf_depth;

	error = gfs2_meta_inode_buffer(dip, &dibh);
	if (!gfs2_assert_withdraw(GFS2_SB(&dip->i_inode), !error)) {
		gfs2_trans_add_bh(dip->i_gl, dibh, 1);
		gfs2_add_inode_blocks(&dip->i_inode, 1);
		gfs2_dinode_out(dip, dibh->b_data);
		brelse(dibh);
	}

	brelse(obh);
	brelse(nbh);

	return error;

fail_lpfree:
	kfree(lp);

fail_brelse:
	brelse(obh);
	brelse(nbh);
	return error;
}

/* 
                                                  
                        
  
                                               
 */

static int dir_double_exhash(struct gfs2_inode *dip)
{
	struct buffer_head *dibh;
	u32 hsize;
	u32 hsize_bytes;
	__be64 *hc;
	__be64 *hc2, *h;
	int x;
	int error = 0;

	hsize = 1 << dip->i_depth;
	hsize_bytes = hsize * sizeof(__be64);

	hc = gfs2_dir_get_hash_table(dip);
	if (IS_ERR(hc))
		return PTR_ERR(hc);

	h = hc2 = kmalloc(hsize_bytes * 2, GFP_NOFS);
	if (!hc2)
		return -ENOMEM;

	error = gfs2_meta_inode_buffer(dip, &dibh);
	if (error)
		goto out_kfree;

	for (x = 0; x < hsize; x++) {
		*h++ = *hc;
		*h++ = *hc;
		hc++;
	}

	error = gfs2_dir_write_data(dip, (char *)hc2, 0, hsize_bytes * 2);
	if (error != (hsize_bytes * 2))
		goto fail;

	gfs2_dir_hash_inval(dip);
	dip->i_hash_cache = hc2;
	dip->i_depth++;
	gfs2_dinode_out(dip, dibh->b_data);
	brelse(dibh);
	return 0;

fail:
	/*                                    */
	gfs2_dir_write_data(dip, (char *)hc, 0, hsize_bytes);
	i_size_write(&dip->i_inode, hsize_bytes);
	gfs2_dinode_out(dip, dibh->b_data);
	brelse(dibh);
out_kfree:
	kfree(hc2);
	return error;
}

/* 
                                                          
                 
                  
  
                                               
                  
                   
                  
 */

static int compare_dents(const void *a, const void *b)
{
	const struct gfs2_dirent *dent_a, *dent_b;
	u32 hash_a, hash_b;
	int ret = 0;

	dent_a = *(const struct gfs2_dirent **)a;
	hash_a = be32_to_cpu(dent_a->de_hash);

	dent_b = *(const struct gfs2_dirent **)b;
	hash_b = be32_to_cpu(dent_b->de_hash);

	if (hash_a > hash_b)
		ret = 1;
	else if (hash_a < hash_b)
		ret = -1;
	else {
		unsigned int len_a = be16_to_cpu(dent_a->de_name_len);
		unsigned int len_b = be16_to_cpu(dent_b->de_name_len);

		if (len_a > len_b)
			ret = 1;
		else if (len_a < len_b)
			ret = -1;
		else
			ret = memcmp(dent_a + 1, dent_b + 1, len_a);
	}

	return ret;
}

/* 
                                               
                       
                                               
                                          
                                            
                                                         
                                          
                                                                         
  
                                                                         
                                                                       
                                                                        
                                                   
  
                                               
 */

static int do_filldir_main(struct gfs2_inode *dip, u64 *offset,
			   void *opaque, filldir_t filldir,
			   const struct gfs2_dirent **darr, u32 entries,
			   int *copied)
{
	const struct gfs2_dirent *dent, *dent_next;
	u64 off, off_next;
	unsigned int x, y;
	int run = 0;
	int error = 0;

	sort(darr, entries, sizeof(struct gfs2_dirent *), compare_dents, NULL);

	dent_next = darr[0];
	off_next = be32_to_cpu(dent_next->de_hash);
	off_next = gfs2_disk_hash2offset(off_next);

	for (x = 0, y = 1; x < entries; x++, y++) {
		dent = dent_next;
		off = off_next;

		if (y < entries) {
			dent_next = darr[y];
			off_next = be32_to_cpu(dent_next->de_hash);
			off_next = gfs2_disk_hash2offset(off_next);

			if (off < *offset)
				continue;
			*offset = off;

			if (off_next == off) {
				if (*copied && !run)
					return 1;
				run = 1;
			} else
				run = 0;
		} else {
			if (off < *offset)
				continue;
			*offset = off;
		}

		error = filldir(opaque, (const char *)(dent + 1),
				be16_to_cpu(dent->de_name_len),
				off, be64_to_cpu(dent->de_inum.no_addr),
				be16_to_cpu(dent->de_type));
		if (error)
			return 1;

		*copied = 1;
	}

	/*                                                                
                                                                        
                 */

	(*offset)++;

	return 0;
}

static void *gfs2_alloc_sort_buffer(unsigned size)
{
	void *ptr = NULL;

	if (size < KMALLOC_MAX_SIZE)
		ptr = kmalloc(size, GFP_NOFS | __GFP_NOWARN);
	if (!ptr)
		ptr = __vmalloc(size, GFP_NOFS, PAGE_KERNEL);
	return ptr;
}

static void gfs2_free_sort_buffer(void *ptr)
{
	if (is_vmalloc_addr(ptr))
		vfree(ptr);
	else
		kfree(ptr);
}

static int gfs2_dir_read_leaf(struct inode *inode, u64 *offset, void *opaque,
			      filldir_t filldir, int *copied, unsigned *depth,
			      u64 leaf_no)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct buffer_head *bh;
	struct gfs2_leaf *lf;
	unsigned entries = 0, entries2 = 0;
	unsigned leaves = 0;
	const struct gfs2_dirent **darr, *dent;
	struct dirent_gather g;
	struct buffer_head **larr;
	int leaf = 0;
	int error, i;
	u64 lfn = leaf_no;

	do {
		error = get_leaf(ip, lfn, &bh);
		if (error)
			goto out;
		lf = (struct gfs2_leaf *)bh->b_data;
		if (leaves == 0)
			*depth = be16_to_cpu(lf->lf_depth);
		entries += be16_to_cpu(lf->lf_entries);
		leaves++;
		lfn = be64_to_cpu(lf->lf_next);
		brelse(bh);
	} while(lfn);

	if (!entries)
		return 0;

	error = -ENOMEM;
	/*
                                                                
                                                              
                                                                
               
  */
	larr = gfs2_alloc_sort_buffer((leaves + entries + 99) * sizeof(void *));
	if (!larr)
		goto out;
	darr = (const struct gfs2_dirent **)(larr + leaves);
	g.pdent = darr;
	g.offset = 0;
	lfn = leaf_no;

	do {
		error = get_leaf(ip, lfn, &bh);
		if (error)
			goto out_free;
		lf = (struct gfs2_leaf *)bh->b_data;
		lfn = be64_to_cpu(lf->lf_next);
		if (lf->lf_entries) {
			entries2 += be16_to_cpu(lf->lf_entries);
			dent = gfs2_dirent_scan(inode, bh->b_data, bh->b_size,
						gfs2_dirent_gather, NULL, &g);
			error = PTR_ERR(dent);
			if (IS_ERR(dent))
				goto out_free;
			if (entries2 != g.offset) {
				fs_warn(sdp, "Number of entries corrupt in dir "
						"leaf %llu, entries2 (%u) != "
						"g.offset (%u)\n",
					(unsigned long long)bh->b_blocknr,
					entries2, g.offset);
					
				error = -EIO;
				goto out_free;
			}
			error = 0;
			larr[leaf++] = bh;
		} else {
			brelse(bh);
		}
	} while(lfn);

	BUG_ON(entries2 != entries);
	error = do_filldir_main(ip, offset, opaque, filldir, darr,
				entries, copied);
out_free:
	for(i = 0; i < leaf; i++)
		brelse(larr[i]);
	gfs2_free_sort_buffer(larr);
out:
	return error;
}

/* 
                                                                  
  
                                                                           
                                                                         
                                                                         
                               
 */
static void gfs2_dir_readahead(struct inode *inode, unsigned hsize, u32 index,
			       struct file_ra_state *f_ra)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_glock *gl = ip->i_gl;
	struct buffer_head *bh;
	u64 blocknr = 0, last;
	unsigned count;

	/*                                                              */
	if (index + MAX_RA_BLOCKS < f_ra->start)
		return;

	f_ra->start = max((pgoff_t)index, f_ra->start);
	for (count = 0; count < MAX_RA_BLOCKS; count++) {
		if (f_ra->start >= hsize) /*                            */
			break;

		last = blocknr;
		blocknr = be64_to_cpu(ip->i_hash_cache[f_ra->start]);
		f_ra->start++;
		if (blocknr == last)
			continue;

		bh = gfs2_getbuf(gl, blocknr, 1);
		if (trylock_buffer(bh)) {
			if (buffer_uptodate(bh)) {
				unlock_buffer(bh);
				brelse(bh);
				continue;
			}
			bh->b_end_io = end_buffer_read_sync;
			submit_bh(READA | REQ_META, bh);
			continue;
		}
		brelse(bh);
	}
}

/* 
                                                                        
                       
                                                                     
                                                   
                                                  
  
                 
 */

static int dir_e_read(struct inode *inode, u64 *offset, void *opaque,
		      filldir_t filldir, struct file_ra_state *f_ra)
{
	struct gfs2_inode *dip = GFS2_I(inode);
	u32 hsize, len = 0;
	u32 hash, index;
	__be64 *lp;
	int copied = 0;
	int error = 0;
	unsigned depth = 0;

	hsize = 1 << dip->i_depth;
	hash = gfs2_dir_offset2hash(*offset);
	index = hash >> (32 - dip->i_depth);

	if (dip->i_hash_cache == NULL)
		f_ra->start = 0;
	lp = gfs2_dir_get_hash_table(dip);
	if (IS_ERR(lp))
		return PTR_ERR(lp);

	gfs2_dir_readahead(inode, hsize, index, f_ra);

	while (index < hsize) {
		error = gfs2_dir_read_leaf(inode, offset, opaque, filldir,
					   &copied, &depth,
					   be64_to_cpu(lp[index]));
		if (error)
			break;

		len = 1 << (dip->i_depth - depth);
		index = (index & ~(len - 1)) + len;
	}

	if (error > 0)
		error = 0;
	return error;
}

int gfs2_dir_read(struct inode *inode, u64 *offset, void *opaque,
		  filldir_t filldir, struct file_ra_state *f_ra)
{
	struct gfs2_inode *dip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct dirent_gather g;
	const struct gfs2_dirent **darr, *dent;
	struct buffer_head *dibh;
	int copied = 0;
	int error;

	if (!dip->i_entries)
		return 0;

	if (dip->i_diskflags & GFS2_DIF_EXHASH)
		return dir_e_read(inode, offset, opaque, filldir, f_ra);

	if (!gfs2_is_stuffed(dip)) {
		gfs2_consist_inode(dip);
		return -EIO;
	}

	error = gfs2_meta_inode_buffer(dip, &dibh);
	if (error)
		return error;

	error = -ENOMEM;
	/*                                                                */
	darr = kmalloc(96 * sizeof(struct gfs2_dirent *), GFP_NOFS);
	if (darr) {
		g.pdent = darr;
		g.offset = 0;
		dent = gfs2_dirent_scan(inode, dibh->b_data, dibh->b_size,
					gfs2_dirent_gather, NULL, &g);
		if (IS_ERR(dent)) {
			error = PTR_ERR(dent);
			goto out;
		}
		if (dip->i_entries != g.offset) {
			fs_warn(sdp, "Number of entries corrupt in dir %llu, "
				"ip->i_entries (%u) != g.offset (%u)\n",
				(unsigned long long)dip->i_no_addr,
				dip->i_entries,
				g.offset);
			error = -EIO;
			goto out;
		}
		error = do_filldir_main(dip, offset, opaque, filldir, darr,
					dip->i_entries, &copied);
out:
		kfree(darr);
	}

	if (error > 0)
		error = 0;

	brelse(dibh);

	return error;
}

/* 
                                       
                       
             
          
  
                                                                     
                                  
  
                 
 */

struct inode *gfs2_dir_search(struct inode *dir, const struct qstr *name)
{
	struct buffer_head *bh;
	struct gfs2_dirent *dent;
	struct inode *inode;

	dent = gfs2_dirent_search(dir, name, gfs2_dirent_find, &bh);
	if (dent) {
		if (IS_ERR(dent))
			return ERR_CAST(dent);
		inode = gfs2_inode_lookup(dir->i_sb, 
				be16_to_cpu(dent->de_type),
				be64_to_cpu(dent->de_inum.no_addr),
				be64_to_cpu(dent->de_inum.no_formal_ino), 0);
		brelse(bh);
		return inode;
	}
	return ERR_PTR(-ENOENT);
}

int gfs2_dir_check(struct inode *dir, const struct qstr *name,
		   const struct gfs2_inode *ip)
{
	struct buffer_head *bh;
	struct gfs2_dirent *dent;
	int ret = -ENOENT;

	dent = gfs2_dirent_search(dir, name, gfs2_dirent_find, &bh);
	if (dent) {
		if (IS_ERR(dent))
			return PTR_ERR(dent);
		if (ip) {
			if (be64_to_cpu(dent->de_inum.no_addr) != ip->i_no_addr)
				goto out;
			if (be64_to_cpu(dent->de_inum.no_formal_ino) !=
			    ip->i_no_formal_ino)
				goto out;
			if (unlikely(IF2DT(ip->i_inode.i_mode) !=
			    be16_to_cpu(dent->de_type))) {
				gfs2_consist_inode(GFS2_I(dir));
				ret = -EIO;
				goto out;
			}
		}
		ret = 0;
out:
		brelse(bh);
	}
	return ret;
}

static int dir_new_leaf(struct inode *inode, const struct qstr *name)
{
	struct buffer_head *bh, *obh;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_leaf *leaf, *oleaf;
	int error;
	u32 index;
	u64 bn;

	index = name->hash >> (32 - ip->i_depth);
	error = get_first_leaf(ip, index, &obh);
	if (error)
		return error;
	do {
		oleaf = (struct gfs2_leaf *)obh->b_data;
		bn = be64_to_cpu(oleaf->lf_next);
		if (!bn)
			break;
		brelse(obh);
		error = get_leaf(ip, bn, &obh);
		if (error)
			return error;
	} while(1);

	gfs2_trans_add_bh(ip->i_gl, obh, 1);

	leaf = new_leaf(inode, &bh, be16_to_cpu(oleaf->lf_depth));
	if (!leaf) {
		brelse(obh);
		return -ENOSPC;
	}
	oleaf->lf_next = cpu_to_be64(bh->b_blocknr);
	brelse(bh);
	brelse(obh);

	error = gfs2_meta_inode_buffer(ip, &bh);
	if (error)
		return error;
	gfs2_trans_add_bh(ip->i_gl, bh, 1);
	gfs2_add_inode_blocks(&ip->i_inode, 1);
	gfs2_dinode_out(ip, bh->b_data);
	brelse(bh);
	return 0;
}

/* 
                                                 
                       
                          
                                        
                               
  
                                               
 */

int gfs2_dir_add(struct inode *inode, const struct qstr *name,
		 const struct gfs2_inode *nip)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct buffer_head *bh;
	struct gfs2_dirent *dent;
	struct gfs2_leaf *leaf;
	int error;

	while(1) {
		dent = gfs2_dirent_search(inode, name, gfs2_dirent_find_space,
					  &bh);
		if (dent) {
			if (IS_ERR(dent))
				return PTR_ERR(dent);
			dent = gfs2_init_dirent(inode, dent, name, bh);
			gfs2_inum_out(nip, dent);
			dent->de_type = cpu_to_be16(IF2DT(nip->i_inode.i_mode));
			if (ip->i_diskflags & GFS2_DIF_EXHASH) {
				leaf = (struct gfs2_leaf *)bh->b_data;
				be16_add_cpu(&leaf->lf_entries, 1);
			}
			brelse(bh);
			error = gfs2_meta_inode_buffer(ip, &bh);
			if (error)
				break;
			gfs2_trans_add_bh(ip->i_gl, bh, 1);
			ip->i_entries++;
			ip->i_inode.i_mtime = ip->i_inode.i_ctime = CURRENT_TIME;
			if (S_ISDIR(nip->i_inode.i_mode))
				inc_nlink(&ip->i_inode);
			gfs2_dinode_out(ip, bh->b_data);
			brelse(bh);
			error = 0;
			break;
		}
		if (!(ip->i_diskflags & GFS2_DIF_EXHASH)) {
			error = dir_make_exhash(inode);
			if (error)
				break;
			continue;
		}
		error = dir_split_leaf(inode, name);
		if (error == 0)
			continue;
		if (error < 0)
			break;
		if (ip->i_depth < GFS2_DIR_MAX_DEPTH) {
			error = dir_double_exhash(ip);
			if (error)
				break;
			error = dir_split_leaf(inode, name);
			if (error < 0)
				break;
			if (error == 0)
				continue;
		}
		error = dir_new_leaf(inode, name);
		if (!error)
			continue;
		error = -ENOSPC;
		break;
	}
	return error;
}


/* 
                                          
                       
                          
  
                                               
 */

int gfs2_dir_del(struct gfs2_inode *dip, const struct dentry *dentry)
{
	const struct qstr *name = &dentry->d_name;
	struct gfs2_dirent *dent, *prev = NULL;
	struct buffer_head *bh;

	/*                                                          
                             */
	dent = gfs2_dirent_search(&dip->i_inode, name, gfs2_dirent_prev, &bh);
	if (!dent) {
		gfs2_consist_inode(dip);
		return -EIO;
	}
	if (IS_ERR(dent)) {
		gfs2_consist_inode(dip);
		return PTR_ERR(dent);
	}
	/*                                                    */
	if (gfs2_dirent_find(dent, name, NULL) == 0) {
		prev = dent;
		dent = (struct gfs2_dirent *)((char *)dent + be16_to_cpu(prev->de_rec_len));
	}

	dirent_del(dip, bh, prev, dent);
	if (dip->i_diskflags & GFS2_DIF_EXHASH) {
		struct gfs2_leaf *leaf = (struct gfs2_leaf *)bh->b_data;
		u16 entries = be16_to_cpu(leaf->lf_entries);
		if (!entries)
			gfs2_consist_inode(dip);
		leaf->lf_entries = cpu_to_be16(--entries);
	}
	brelse(bh);

	if (!dip->i_entries)
		gfs2_consist_inode(dip);
	dip->i_entries--;
	dip->i_inode.i_mtime = dip->i_inode.i_ctime = CURRENT_TIME;
	if (S_ISDIR(dentry->d_inode->i_mode))
		drop_nlink(&dip->i_inode);
	mark_inode_dirty(&dip->i_inode);

	return 0;
}

/* 
                                                          
                       
             
              
  
                                                                         
                                                      
                                  
  
                 
 */

int gfs2_dir_mvino(struct gfs2_inode *dip, const struct qstr *filename,
		   const struct gfs2_inode *nip, unsigned int new_type)
{
	struct buffer_head *bh;
	struct gfs2_dirent *dent;
	int error;

	dent = gfs2_dirent_search(&dip->i_inode, filename, gfs2_dirent_find, &bh);
	if (!dent) {
		gfs2_consist_inode(dip);
		return -EIO;
	}
	if (IS_ERR(dent))
		return PTR_ERR(dent);

	gfs2_trans_add_bh(dip->i_gl, bh, 1);
	gfs2_inum_out(nip, dent);
	dent->de_type = cpu_to_be16(new_type);

	if (dip->i_diskflags & GFS2_DIF_EXHASH) {
		brelse(bh);
		error = gfs2_meta_inode_buffer(dip, &bh);
		if (error)
			return error;
		gfs2_trans_add_bh(dip->i_gl, bh, 1);
	}

	dip->i_inode.i_mtime = dip->i_inode.i_ctime = CURRENT_TIME;
	gfs2_dinode_out(dip, bh->b_data);
	brelse(bh);
	return 0;
}

/* 
                                             
                      
                                                 
                                            
                            
                                              
                                                                    
  
                 
 */

static int leaf_dealloc(struct gfs2_inode *dip, u32 index, u32 len,
			u64 leaf_no, struct buffer_head *leaf_bh,
			int last_dealloc)
{
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
	struct gfs2_leaf *tmp_leaf;
	struct gfs2_rgrp_list rlist;
	struct buffer_head *bh, *dibh;
	u64 blk, nblk;
	unsigned int rg_blocks = 0, l_blocks = 0;
	char *ht;
	unsigned int x, size = len * sizeof(u64);
	int error;

	error = gfs2_rindex_update(sdp);
	if (error)
		return error;

	memset(&rlist, 0, sizeof(struct gfs2_rgrp_list));

	ht = kzalloc(size, GFP_NOFS);
	if (!ht)
		return -ENOMEM;

	if (!gfs2_qadata_get(dip)) {
		error = -ENOMEM;
		goto out;
	}

	error = gfs2_quota_hold(dip, NO_QUOTA_CHANGE, NO_QUOTA_CHANGE);
	if (error)
		goto out_put;

	/*                              */
	bh = leaf_bh;

	for (blk = leaf_no; blk; blk = nblk) {
		if (blk != leaf_no) {
			error = get_leaf(dip, blk, &bh);
			if (error)
				goto out_rlist;
		}
		tmp_leaf = (struct gfs2_leaf *)bh->b_data;
		nblk = be64_to_cpu(tmp_leaf->lf_next);
		if (blk != leaf_no)
			brelse(bh);

		gfs2_rlist_add(dip, &rlist, blk);
		l_blocks++;
	}

	gfs2_rlist_alloc(&rlist, LM_ST_EXCLUSIVE);

	for (x = 0; x < rlist.rl_rgrps; x++) {
		struct gfs2_rgrpd *rgd;
		rgd = rlist.rl_ghs[x].gh_gl->gl_object;
		rg_blocks += rgd->rd_length;
	}

	error = gfs2_glock_nq_m(rlist.rl_rgrps, rlist.rl_ghs);
	if (error)
		goto out_rlist;

	error = gfs2_trans_begin(sdp,
			rg_blocks + (DIV_ROUND_UP(size, sdp->sd_jbsize) + 1) +
			RES_DINODE + RES_STATFS + RES_QUOTA, l_blocks);
	if (error)
		goto out_rg_gunlock;

	bh = leaf_bh;

	for (blk = leaf_no; blk; blk = nblk) {
		if (blk != leaf_no) {
			error = get_leaf(dip, blk, &bh);
			if (error)
				goto out_end_trans;
		}
		tmp_leaf = (struct gfs2_leaf *)bh->b_data;
		nblk = be64_to_cpu(tmp_leaf->lf_next);
		if (blk != leaf_no)
			brelse(bh);

		gfs2_free_meta(dip, blk, 1);
		gfs2_add_inode_blocks(&dip->i_inode, -1);
	}

	error = gfs2_dir_write_data(dip, ht, index * sizeof(u64), size);
	if (error != size) {
		if (error >= 0)
			error = -EIO;
		goto out_end_trans;
	}

	error = gfs2_meta_inode_buffer(dip, &dibh);
	if (error)
		goto out_end_trans;

	gfs2_trans_add_bh(dip->i_gl, dibh, 1);
	/*                                                                
                                                         */
	if (last_dealloc)
		dip->i_inode.i_mode = S_IFREG;
	gfs2_dinode_out(dip, dibh->b_data);
	brelse(dibh);

out_end_trans:
	gfs2_trans_end(sdp);
out_rg_gunlock:
	gfs2_glock_dq_m(rlist.rl_rgrps, rlist.rl_ghs);
out_rlist:
	gfs2_rlist_free(&rlist);
	gfs2_quota_unhold(dip);
out_put:
	gfs2_qadata_put(dip);
out:
	kfree(ht);
	return error;
}

/* 
                                                                    
                      
  
                                                         
                                              
  
                 
 */

int gfs2_dir_exhash_dealloc(struct gfs2_inode *dip)
{
	struct buffer_head *bh;
	struct gfs2_leaf *leaf;
	u32 hsize, len;
	u32 index = 0, next_index;
	__be64 *lp;
	u64 leaf_no;
	int error = 0, last;

	hsize = 1 << dip->i_depth;

	lp = gfs2_dir_get_hash_table(dip);
	if (IS_ERR(lp))
		return PTR_ERR(lp);

	while (index < hsize) {
		leaf_no = be64_to_cpu(lp[index]);
		if (leaf_no) {
			error = get_leaf(dip, leaf_no, &bh);
			if (error)
				goto out;
			leaf = (struct gfs2_leaf *)bh->b_data;
			len = 1 << (dip->i_depth - be16_to_cpu(leaf->lf_depth));

			next_index = (index & ~(len - 1)) + len;
			last = ((next_index >= hsize) ? 1 : 0);
			error = leaf_dealloc(dip, index, len, leaf_no, bh,
					     last);
			brelse(bh);
			if (error)
				goto out;
			index = next_index;
		} else
			index++;
	}

	if (index != hsize) {
		gfs2_consist_inode(dip);
		error = -EIO;
	}

out:

	return error;
}

/* 
                                                                               
                                 
                                                  
  
                                                       
 */

int gfs2_diradd_alloc_required(struct inode *inode, const struct qstr *name)
{
	struct gfs2_dirent *dent;
	struct buffer_head *bh;

	dent = gfs2_dirent_search(inode, name, gfs2_dirent_find_space, &bh);
	if (!dent) {
		return 1;
	}
	if (IS_ERR(dent))
		return PTR_ERR(dent);
	brelse(bh);
	return 0;
}

