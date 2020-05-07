#ifndef __CMAP_COMMON_H__
#define __CMAP_COMMON_H__

#include "cmap-common-mem.h"
#include "cmap-common-map.h"
#include "cmap-common-list.h"
#include "cmap-common-fn.h"
#include "cmap-common-kernel.h"

#define CMAP_T (1 == 1)
#define CMAP_F (1 == 0)

#define CMAP_CALL(e, fn) (e) -> fn(e)
#define CMAP_CALL_ARGS(e, fn, args...) (e) -> fn(e, args)

#endif
