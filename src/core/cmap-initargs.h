#ifndef __CMAP_INITARGS_H__
#define __CMAP_INITARGS_H__

typedef struct
{
  unsigned char nature;

  CMAP_MAP * prototype;

  CMAP_PROC_CTX * proc_ctx;
} CMAP_INITARGS;

#endif
