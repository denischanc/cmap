#ifndef __CMAP_INT_H__
#define __CMAP_INT_H__

#include <cmap/core.h>
#include "cmap-map.h"
#include <sys/types.h>

struct CMAP_INT_s
{
  CMAP_MAP super;

  void * internal;

  int64_t (*get)(CMAP_INT * this);
  void (*set)(CMAP_INT * this, int64_t val);
};

typedef struct
{
  const char * nature;

  int64_t (*get)(CMAP_INT * this);
  void (*set)(CMAP_INT * this, int64_t val);

  CMAP_INT * (*create)(const char * aisle);
  void (*init)(CMAP_INT * int_);
  CMAP_MAP * (*delete)(CMAP_INT * int_);
} CMAP_INT_PUBLIC;

extern const CMAP_INT_PUBLIC cmap_int_public;

#endif
