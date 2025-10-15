
#include "cmap-parser-cmp.h"

#include <stdlib.h>
#include "cmap-parser-util.h"
#include "cmap-parser-part.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(name, op) \
static char * name(char * map_l, char * map_r) \
{ \
  APPEND_INSTRUCTION_ARGS( \
    "return (cmap_cmp(%s, %s) " #op " 0);", map_l, map_r); \
  \
  free(map_l); \
  free(map_r); \
 \
  return cmap_parser_part_public.function_cmp(); \
}

CMAP_PARSER_CMP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define UNIQUE_IMPL(fn, op) \
static char * fn(char * map) \
{ \
  APPEND_INSTRUCTION_ARGS("if(cmap_nature(%s) == CMAP_INT_NATURE)", map); \
  APPEND_INSTRUCTION_ARGS( \
    SPACE "return (cmap_int_get((CMAP_INT *)%s) " #op " 0);", map); \
  APPEND_INSTRUCTION_ARGS("else return (%s " #op " NULL);", map); \
 \
  free(map); \
 \
  return cmap_parser_part_public.function_cmp(); \
}

UNIQUE_IMPL(cmp_unique, !=)
UNIQUE_IMPL(cmp_unique_not, ==)

/*******************************************************************************
*******************************************************************************/

static char * or_and(char * cmp_call_l, char * cmp_call_r, const char * op)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "(%s %s %s)",
    cmp_call_l, op, cmp_call_r);
  free(cmp_call_l);
  free(cmp_call_r);
  return ret;
}

static char * or(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "||");
}

static char * and(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "&&");
}

/*******************************************************************************
*******************************************************************************/

#define SET(name, op) name,

const CMAP_PARSER_CMP_PUBLIC cmap_parser_cmp_public =
{
  CMAP_PARSER_CMP_LOOP(SET) cmp_unique, cmp_unique_not,
  or, and
};
