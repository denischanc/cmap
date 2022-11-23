#ifndef ___CMAP_FN_H___
#define ___CMAP_FN_H___

#include <cmap/core.h>
#include <cmap/proc-ctx.h>

typedef CMAP_MAP * (*CMAP_FN_TPL)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args);

extern const char * CMAP_PROTOTYPE_NAME;

#endif
