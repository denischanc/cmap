#ifndef __CMAP_CLI_H__
#define __CMAP_CLI_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx);
} CMAP_CLI_PUBLIC;

extern const CMAP_CLI_PUBLIC cmap_cli_public;

#endif
