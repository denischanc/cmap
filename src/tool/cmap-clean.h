#ifndef __CMAP_CLEAN_H__
#define __CMAP_CLEAN_H__

typedef struct
{
  void (*clean)();
} CMAP_CLEAN_PUBLIC;

extern const CMAP_CLEAN_PUBLIC cmap_clean_public;

#endif
