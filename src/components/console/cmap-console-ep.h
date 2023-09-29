#ifndef __CMAP_CONSOLE_EP_H__
#define __CMAP_CONSOLE_EP_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

CMAP_MAP * cmap_console_ep_public_info(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args);
CMAP_MAP * cmap_console_ep_public_error(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args);

#endif
