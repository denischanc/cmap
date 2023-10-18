#ifndef __CMAP_CLI_EP_H__
#define __CMAP_CLI_EP_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx, int argc, char ** argv);
} CMAP_CLI_EP_PUBLIC;

extern const CMAP_CLI_EP_PUBLIC cmap_cli_ep_public;

#endif
