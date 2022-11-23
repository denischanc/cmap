#ifndef __CMAP_CONSOLE_H__
#define __CMAP_CONSOLE_H__

#include <cmap/core.h>
#include <cmap/proc-ctx.h>

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx);
} CMAP_CONSOLE_PUBLIC;

extern const CMAP_CONSOLE_PUBLIC cmap_console_public;

#endif
