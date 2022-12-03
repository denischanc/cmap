#ifndef __CMAP_FN_TYPE_H__
#define __CMAP_FN_TYPE_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

typedef struct CMAP_FN_s CMAP_FN;

typedef CMAP_MAP * (*CMAP_FN_TPL)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args);

#endif
