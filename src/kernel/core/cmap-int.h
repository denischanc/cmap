#ifndef __CMAP_INT_H__
#define __CMAP_INT_H__

#include "cmap-core.h"
#include "cmap-map.h"
#include <sys/types.h>

extern const char * CMAP_INT_NATURE;

struct CMAP_INT_s
{
  CMAP_MAP super_;

  void * internal_;

  int64_t (*get)(CMAP_INT * this);
  void (*set)(CMAP_INT * this, int64_t val);
};

int64_t cmap_int__get(CMAP_INT * this);
void cmap_int__set(CMAP_INT * this, int64_t val);

CMAP_INT * cmap_int_create();
void cmap_int_init(CMAP_INT * _int);
void cmap_int_delete(CMAP_INT * _int);

#endif
