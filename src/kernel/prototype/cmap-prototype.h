#ifndef __CMAP_PROTOTYPE_H__
#define __CMAP_PROTOTYPE_H__

typedef struct
{
  void (*init)();
} CMAP_PROTOTYPE_PUBLIC;

extern const CMAP_PROTOTYPE_PUBLIC cmap_prototype_public;

#endif
