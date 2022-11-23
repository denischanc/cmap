#ifndef __CMAP_GLOBAL_ENV_H__
#define __CMAP_GLOBAL_ENV_H__

#include "cmap-map.h"
#include "cmap-proc-ctx.h"

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx);
} CMAP_GLOBAL_ENV_PUBLIC;

extern const CMAP_GLOBAL_ENV_PUBLIC cmap_global_env_public;

#endif
