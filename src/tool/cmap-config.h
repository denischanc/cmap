#ifndef __CMAP_CONFIG_H__
#define __CMAP_CONFIG_H__

#include "cmap-config-define.h"

typedef struct
{
  CMAP_CONFIG_LOOP(CMAP_CONFIG_BOOL_DECL, CMAP_CONFIG_STRING_DECL,
    CMAP_CONFIG_STRINGS_DECL)

  void (*clean)();
} CMAP_CONFIG_PUBLIC;

extern const CMAP_CONFIG_PUBLIC cmap_config_public;

#endif
