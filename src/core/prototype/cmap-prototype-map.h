#ifndef __CMAP_PROTOTYPE_MAP_H__
#define __CMAP_PROTOTYPE_MAP_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

void cmap_prototype_map_require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx);
void cmap_prototype_map_init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx);

#endif
