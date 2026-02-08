#ifndef __CMAP_CONSOLE_H__
#define __CMAP_CONSOLE_H__

#include "cmap-console-define.h"

typedef struct
{
  void (*info)(const char * txt, ...);
  void (*error)(const char * txt, ...);
} CMAP_CONSOLE_PUBLIC;

extern const CMAP_CONSOLE_PUBLIC cmap_console_public;

#endif
