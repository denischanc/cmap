#ifndef __CMAP_FN_NAME_H__
#define __CMAP_FN_NAME_H__

#include "cmap-fn-name-define.h"

typedef struct
{
  void (*clean)();

  char * (*name)();

  CMAP_FN_NAME_LOOP(CMAP_FN_NAME_DECL)
} CMAP_FN_NAME_PUBLIC;

extern const CMAP_FN_NAME_PUBLIC cmap_fn_name_public;

#endif
