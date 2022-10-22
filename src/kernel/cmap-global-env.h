#ifndef __CMAP_GLOBAL_ENV_H__
#define __CMAP_GLOBAL_ENV_H__

#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * (*init)();
  CMAP_MAP * (*instance)();
} CMAP_GLOBAL_ENV_PUBLIC;

extern const CMAP_GLOBAL_ENV_PUBLIC cmap_global_env_public;

#endif
