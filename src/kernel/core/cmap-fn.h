#ifndef __CMAP_FN_H__
#define __CMAP_FN_H__

#include "cmap-core.h"
#include "cmap-map.h"

extern const char * CMAP_FN_NATURE;

struct CMAP_FN_s
{
  CMAP_MAP super_;

  void * internal_;

  CMAP_MAP * features_;

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args, const char * aisle);
};

CMAP_MAP * cmap_fn__process(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args);

CMAP_MAP * cmap_fn__new(CMAP_FN * this, CMAP_LIST * args, const char * aisle);

typedef CMAP_MAP * (*CMAP_FN_TPL)(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args);

CMAP_FN * cmap_fn_create(CMAP_FN_TPL process, const char * aisle);
void cmap_fn_init(CMAP_FN * fn, CMAP_FN_TPL process);
CMAP_MAP * cmap_fn_delete(CMAP_FN * fn);

#endif
