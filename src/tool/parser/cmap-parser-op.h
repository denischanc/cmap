#ifndef __CMAP_PARSER_OP_H__
#define __CMAP_PARSER_OP_H__

#include "cmap-parser-op-define.h"

typedef struct
{
  CMAP_PARSER_OP_LOOP(CMAP_PARSER_OP_DECL)

  CMAP_PARSER_OP_SELF_LOOP(CMAP_PARSER_OP_SELF_DECL)

  CMAP_PARSER_OP_UNIQUE_LOOP(CMAP_PARSER_OP_UNIQUE_DECL)
} CMAP_PARSER_OP_PUBLIC;

extern const CMAP_PARSER_OP_PUBLIC cmap_parser_op_public;

#endif
