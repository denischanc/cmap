
#include "cmap-parser-op.h"

#include <stdlib.h>
#include "cmap-parser-util.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(name) \
static char * op_##name(char * map_l, char * map_r) \
{ \
  char * map_name = NEXT_NAME_OP(); \
 \
  PREPEND_MAP_VAR(map_name); \
  APPEND_INSTRUCTION_ARGS( \
    "%s = cmap_" #name "(%s, %s, proc_ctx);", map_name, map_l, map_r); \
  APPEND_LF(); \
 \
  free(map_l); \
  free(map_r); \
 \
  return map_name; \
}

CMAP_PARSER_OP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define SELF_IMPL(name) \
static void op_##name(char * map_dst, char * map_src) \
{ \
  APPEND_INSTRUCTION_ARGS("cmap_" #name "(%s, %s);", map_dst, map_src); \
  APPEND_LF(); \
 \
  free(map_dst); \
  free(map_src); \
}

CMAP_PARSER_OP_SELF_LOOP(SELF_IMPL)

/*******************************************************************************
*******************************************************************************/

#define UNIQUE_IMPL(name) \
static void op_##name(char * map) \
{ \
  APPEND_INSTRUCTION_ARGS("cmap_" #name "(%s);", map); \
  APPEND_LF(); \
 \
  free(map); \
}

CMAP_PARSER_OP_UNIQUE_LOOP(UNIQUE_IMPL)

/*******************************************************************************
*******************************************************************************/

#define SET(name) op_##name,

const CMAP_PARSER_OP_PUBLIC cmap_parser_op_public =
{
  CMAP_PARSER_OP_LOOP(SET)
  CMAP_PARSER_OP_SELF_LOOP(SET)
  CMAP_PARSER_OP_UNIQUE_LOOP(SET)
};
