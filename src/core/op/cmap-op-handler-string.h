#ifndef __CMAP_OP_HANDLER_STRING_H__
#define __CMAP_OP_HANDLER_STRING_H__

#include "cmap-string-type.h"
#include "cmap-map-type.h"

typedef struct
{
  void (*append)(CMAP_STRING * dst, CMAP_MAP * src);
} CMAP_OP_HANDLER_STRING_PUBLIC;

extern const CMAP_OP_HANDLER_STRING_PUBLIC cmap_op_handler_string_public;

#endif
