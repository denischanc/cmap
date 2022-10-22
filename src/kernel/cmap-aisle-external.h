#ifndef __CMAP_AISLE_EXTERNAL_H__
#define __CMAP_AISLE_EXTERNAL_H__

typedef struct
{
  const char * local;
  const char * kernel;
  const char * global;
  const char * stack;
} CMAP_AISLE_PUBLIC;

extern const CMAP_AISLE_PUBLIC cmap_aisle_public;

#endif
