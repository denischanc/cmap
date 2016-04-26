#ifndef __CMAP_FW_H__
#define __CMAP_FW_H__

#include <stdarg.h>

#include "cmap-fw-define.h"

void cmap_fw_set_split(CMAP_MAP * map, const char * keys, CMAP_MAP * val);
CMAP_MAP * cmap_fw_get_split(CMAP_MAP * map, const char * keys);

CMAP_MAP * cmap_fw_vproc(CMAP_MAP * map, const char * fn_name, va_list args);
CMAP_MAP * cmap_fw_proc(CMAP_MAP * map, const char * fn_name, ...);
CMAP_MAP * cmap_fw_proc_split(CMAP_MAP * map, const char * fn_names, ...);

#endif
