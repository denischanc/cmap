#ifndef __CMAP_OPTION_H__
#define __CMAP_OPTION_H__

#include "cmap-option-define.h"

typedef struct
{
  CMAP_OPTION_LOOP(CMAP_OPTION_BOOL_DECL, CMAP_OPTION_STRING_DECL)
} CMAP_OPTION_PUBLIC;

extern const CMAP_OPTION_PUBLIC cmap_option_public;

#endif
