#ifndef __CMAP_INITARGS_H__
#define __CMAP_INITARGS_H__

typedef struct
{
  const char * nature;

  CMAP_MAP * prototype;

  CMAP_LIFECYCLE * allocator;

  CMAP_PROC_CTX * proc_ctx;
} CMAP_INITARGS;

#endif
