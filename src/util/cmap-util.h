#ifndef __CMAP_UTIL_H__
#define __CMAP_UTIL_H__

#include <stdarg.h>
#include <stdint.h>
#include "cmap-list-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-map-type.h"

CMAP_LIST * cmap_util_fill_list(CMAP_LIST * list, ...);
CMAP_LIST * cmap_util_vfill_list(CMAP_LIST * list, va_list maps);

CMAP_LIST * cmap_util_to_list(CMAP_PROC_CTX * proc_ctx, ...);
CMAP_LIST * cmap_util_vto_list(CMAP_PROC_CTX * proc_ctx, va_list maps);

CMAP_MAP * cmap_util_to_map(CMAP_PROC_CTX * proc_ctx, ...);
CMAP_MAP * cmap_util_vto_map(CMAP_PROC_CTX * proc_ctx, va_list key_maps);

CMAP_MAP * cmap_util_copy(CMAP_MAP * dst, CMAP_MAP * src);

char * cmap_util_strdup(const char * src);

uint64_t cmap_util_time_us();

#endif
