#ifndef __CMAP_ARRAY_H__
#define __CMAP_ARRAY_H__

#include "cmap-map.h"

extern const char * CMAP_ARRAY_NATURE;

typedef struct CMAP_ARRAY_s CMAP_ARRAY;

struct CMAP_ARRAY_s
{
  CMAP_MAP super_;

  void * internal_;

  void (*set)(CMAP_ARRAY * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_ARRAY * this, int i);
};

CMAP_ARRAY * cmap_array_create();
void cmap_array_init(CMAP_ARRAY * array);
void cmap_array_delete(CMAP_ARRAY * array);

#endif
