#ifndef __CMAP_MODULE_H__
#define __CMAP_MODULE_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx.h"

CMAP_MAP * cmap_module_load(const char * path_so, CMAP_PROC_CTX * proc_ctx);

void cmap_module_load_from_config(CMAP_PROC_CTX * proc_ctx);

#endif
