#ifndef __CMAP_FN_H__
#define __CMAP_FN_H__

#include "cmap-core.h"
#include "cmap-map.h"
#include "cmap-fn-define.h"

struct CMAP_FN_s
{
  CMAP_MAP super;

  void * internal;

  CMAP_MAP * features;

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args, const char * aisle);
};

typedef struct {
  const char * nature, * prototype_name;

  CMAP_FN * (*create)(CMAP_FN_TPL process, const char * aisle);
  void (*init)(CMAP_FN * fn, CMAP_FN_TPL process);
  CMAP_MAP * (*delete)(CMAP_FN * fn);

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args, const char * aisle);
} CMAP_FN_PUBLIC;

extern const CMAP_FN_PUBLIC cmap_fn_public;

#endif
