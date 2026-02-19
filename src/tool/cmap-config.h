#ifndef __CMAP_CONFIG_H__
#define __CMAP_CONFIG_H__

#include "cmap-config-define.h"

CMAP_CONFIG_LOOP(CMAP_CONFIG_BOOL_DECL, CMAP_CONFIG_STRING_DECL,
  CMAP_CONFIG_STRINGS_DECL)

void cmap_config_clean();

#endif
