#ifndef __CMAP_GLOBAL_ENV_H__
#define __CMAP_GLOBAL_ENV_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx, int argc, char ** argv);
} CMAP_GLOBAL_ENV_PUBLIC;

extern const CMAP_GLOBAL_ENV_PUBLIC cmap_global_env_public;

#endif
