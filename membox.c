/*
 * Description:                 Memory controlling functions.
 * File:                        membox.c
 * Author:                      Li Yi <liyi@net.pku.edu.cn>
 * Date & Time:                 Mon Apr  7 23:19:15 2008
 */

#include        "membox.h"
#include        <sys/mman.h>

inline unsigned long
membox_get_offset (membox_struct *membox, void *addr)
{
  unsigned long offset = 0;

  offset = (unsigned long)addr - (unsigned long)membox->pool;

  return offset;
}

inline void *
membox_get_addr (membox_struct *membox, unsigned long offset)
{
  void  *addr = NULL;

  addr = (void *)((unsigned long)membox->pool + offset);

  return addr;
}

int
membox_init (membox_struct *membox, size_t size, membox_mode mode)
{
  char  name[64];

  membox->mode = mode;
  assert (size > 0);
  strcpy (name, membox->name);
  strcat (name, ".membox.bin");
  switch (mode) {
  case MMAP_RWC:
    membox->fmap = open (name, O_RDWR | O_CREAT, 0644);
    assert (membox->fmap != -1);
    membox->pool = NULL;
    assert (lseek (membox->fmap, size - 1, SEEK_SET) != -1);
    assert (write (membox->fmap, " ", 1) != -1);
    assert ((membox->pool =
             mmap (0, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                   membox->fmap, 0))
            != MAP_FAILED);
    membox->size = membox->pool_left = size;
    break;
  case MMAP_RD:
    membox->fmap = open (name, O_RDONLY);
    assert (membox->fmap != -1);
    membox->pool = NULL;
    assert ((membox->pool =
             mmap (0, size, PROT_READ, MAP_SHARED, membox->fmap, 0))
            != MAP_FAILED);
    membox->size = size;
    membox->pool_left = 0;
    break;
  case MMAP_WR:
    membox->fmap = open (name, O_RDWR);
    assert (membox->fmap != -1);
    membox->pool = NULL;
    assert ((membox->pool =
             mmap (0, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                   membox->fmap, 0))
            != MAP_FAILED);
    membox->size = size;
    membox->pool_left = 0;
    break;
  case MALLOC:
    membox->pool = malloc (size);
    membox->size = membox->pool_left = size;
    break;
  case SHM_RWC:
  case SHM_RD:
  case SHM_WR:
    break;
  }
  assert (membox->pool != NULL);
  membox->pool_head = membox->pool;

  return 0;
}

#if INCMEMBOX
void *
membox_alloc (membox_struct *membox, size_t size)
{
  if (membox->size <= 0) {
    WARNING("membox not initialized!");
    return NULL;
  }

  if (membox->pool_left < size) {
    size_t      used = membox->pool_head - membox->pool;

    membox->pool = realloc (membox->pool, membox->size + INCREMSIZE);
    if (membox->pool == NULL) {
      ERROR("membox_alloc: realloc error");
    }
    membox->size = membox->size + INCREMSIZE;
    membox->pool_head = membox->pool;
    membox->pool_left = membox->size - used;
  }
  
  char  *ptr = membox->pool_head;
  membox->pool_head += size;
  membox->pool_left -= size;

  return ptr;
}
#else
void *
membox_alloc (membox_struct *membox, size_t size)
{
  if (membox->size <= 0) {
    WARNING("membox_alloc: not initialized");
    return NULL;
  }

  if (membox->pool_left < size) {
    WARNING("membox_alloc: not enough space");
    return NULL;
  }
  
  char  *ptr = membox->pool_head;
  membox->pool_head += size;
  membox->pool_left -= size;

  return ptr;
}
#endif /* INCMEMBOX */

void
membox_free (membox_struct *membox)
{
  int   ret;

  if (membox->pool != NULL) {
    switch (membox->mode) {
    case MMAP_RWC:
    case MMAP_WR:
      ret = msync (membox->pool, membox->size, MS_SYNC);
      if (ret == -1) {
        perror ("membox: msync error:");
      }
    case MMAP_RD:
      ret = munmap (membox->pool, membox->size);
      if (ret == -1) {
        perror ("membox: msync error:");
      }
      close (membox->fmap);
      break;
    case MALLOC:
      free (membox->pool);
      break;
    case SHM_RWC:
    case SHM_RD:
    case SHM_WR:
      break;
    }
  }
  membox->size = 0;
  membox->pool = NULL;
  membox->pool_head = NULL;
  membox->pool_left = 0;

  return;
}

void
membox_stat (membox_struct *membox)
{
  OUTPUT(membox statistic infomation:);
  fprintf (stdout, "\tpool size: %15lu\n", membox->size);
  fprintf (stdout, "\tpool used: %15lu\n", membox->size - membox->pool_left);
  fprintf (stdout, "\tpool left: %15lu\n", membox->pool_left);
  fprintf (stdout, "\tpool head: %15lu - %15lX\n",
           (unsigned long)membox->pool_head, (unsigned long)membox->pool_head);

  return;
}

void
membox_reset (membox_struct *membox)
{
  membox->pool_head = membox->pool;
  membox->pool_left = membox->size;

  return;
}

int
membox_resize (membox_struct *membox, size_t size)
{
  return 0;
}
