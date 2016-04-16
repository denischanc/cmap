#ifndef __CMAP_FW_H__
#define __CMAP_FW_H__

#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-fn.h"

#define _M_() cmap_map_create()
#define _L_(size_inc) cmap_list_create(size_inc)
#define _F_(process) cmap_fn_create(process)

#endif
