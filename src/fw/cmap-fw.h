#ifndef __CMAP_FW_H__
#define __CMAP_FW_H__

#include <stdarg.h>

#include "cmap-core.h"
#include "cmap-fw-define.h"

CMAP_MAP * cmap_map(const char * aisle);
CMAP_LIST * cmap_list(int size_inc, const char * aisle);
CMAP_FN * cmap_fn(CMAP_FN_TPL process, const char * aisle);
CMAP_STRING * cmap_string(const char * val, int size_inc, const char * aisle);
CMAP_INT * cmap_int(const char * aisle);

const char * cmap_nature(CMAP_MAP * map);
CMAP_MAP * cmap_delete(CMAP_MAP * map);
void cmap_set(CMAP_MAP * map, const char * key, CMAP_MAP * val);
CMAP_MAP * cmap_get(CMAP_MAP * map, const char * key);

CMAP_MAP * cmap_process(CMAP_FN * fn, CMAP_MAP * map, CMAP_LIST * args);

void cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val);
CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i);

void cmap_fw_set_split(CMAP_MAP * map, const char * keys, CMAP_MAP * val);
CMAP_MAP * cmap_fw_get_split(CMAP_MAP * map, const char * keys);

CMAP_MAP * cmap_fw_vproc(CMAP_MAP * map, const char * fn_name, va_list args);
CMAP_MAP * cmap_fw_proc(CMAP_MAP * map, const char * fn_name, ...);
CMAP_MAP * cmap_fw_proc_split(CMAP_MAP * map, const char * fn_names, ...);

#endif
