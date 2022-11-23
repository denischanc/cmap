#ifndef __CMAP_PROTOTYPE_STRING_H__
#define __CMAP_PROTOTYPE_STRING_H__

#include <cmap/core.h>
#include "cmap-proc-ctx.h"

typedef struct
{
  CMAP_MAP * (*require)(CMAP_PROC_CTX * proc_ctx);
  CMAP_MAP * (*instance)(CMAP_PROC_CTX * proc_ctx);
} CMAP_PROTOTYPE_STRING_PUBLIC;

extern const CMAP_PROTOTYPE_STRING_PUBLIC cmap_prototype_string_public;

#endif
