#ifndef __CMAP_CONSOLE_H__
#define __CMAP_CONSOLE_H__

#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * (*create)();
} CMAP_CONSOLE_PUBLIC;

extern const CMAP_CONSOLE_PUBLIC cmap_console_public;

#endif
