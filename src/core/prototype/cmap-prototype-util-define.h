#ifndef __CMAP_PROTOTYPE_UTIL_DEFINE_H__
#define __CMAP_PROTOTYPE_UTIL_DEFINE_H__

#include "cmap-common.h"

#define CMAP_PROTO_SET_FN(proto, fn_name, fn, proc_ctx) \
  CMAP_SET(proto, fn_name, CMAP_KERNEL_FN(fn, proc_ctx))

#endif
