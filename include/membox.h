/*
 * Description:                 Memory controlling functions. This membox is
 *                              static, thus can not be freed while using it.
 *                              It is for large memory purpose, like urldict,
 *                              or url_links.
 * File:                        membox.h
 * Author:                      Li Yi <liyi@net.pku.edu.cn>
 * Date & Time:                 Mon Apr  7 23:13:37 2008
 */

#ifndef MEMBOX_H
#define MEMBOX_H

#include        "common.h"

#define INCMEMBOX       0

#define MEMBOXSIZE      (1024UL * 1024UL * 1024UL)

#if     INCMEMBOX
#define INCREMSIZE      (1024UL * 128UL)
#endif

typedef enum _membox_mode
  {
    SHM_RWC,
    SHM_RD,
    SHM_WR,
    MMAP_RWC,
    MMAP_RD,
    MMAP_WR,
    MALLOC
  } membox_mode;


typedef struct _membox_struct
{
  char          name[32];
  int           fmap;
  size_t        size;
  membox_mode   mode;
  void          *pool;
  void          *pool_head;
  size_t        pool_left;
} membox_struct;

unsigned long
membox_get_offset (membox_struct *membox, void *addr);

void *
membox_get_addr (membox_struct *membox, unsigned long offset);

int
membox_init (membox_struct *membox, size_t size, membox_mode mode);

void *
membox_alloc (membox_struct *membox, size_t size);

int
membox_resize (membox_struct *membox, size_t new_size);

void
membox_reset (membox_struct *membox);

void
membox_free (membox_struct *membox);

void
membox_stat (membox_struct *membox);

int
membox_mmap (membox_struct *membox);

#endif  /* MEMBOX_H */
