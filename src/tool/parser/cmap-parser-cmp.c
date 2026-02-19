
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

static char * cmp_unique_common(char * map, char not)
{
  APPEND_INSTRUCTION_ARGS("return %scmap_true(%s);", not ? "!" : "", map);

  free(map);

  return cmap_parser_part_public.function_cmp();
}

static char * cmp_unique(char * map)
{
  return cmp_unique_common(map, (1 == 0));
}

static char * cmp_unique_not(char * map)
{
  return cmp_unique_common(map, (1 == 1));
}

/*******************************************************************************
*******************************************************************************/

static char * init(char * cmp_call)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, "(%s)", cmp_call);
  free(cmp_call);
  return ret;
}

static char * or_and(char * cmp_call_l, char * cmp_call_r, const char * op,
  char simple)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, "%s %s %s%s%s",
    cmp_call_l, op, simple ? "" : "(", cmp_call_r, simple ? "" : ")");
  free(cmp_call_l);
  free(cmp_call_r);
  return ret;
}

static char * or_simple(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "||", (1 == 1));
}

static char * or(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "||", (1 == 0));
}

static char * and_simple(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "&&", (1 == 1));
}

static char * and(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "&&", (1 == 0));
}

/*******************************************************************************
*******************************************************************************/

#define SET(name, op) name,

const CMAP_PARSER_CMP_PUBLIC cmap_parser_cmp_public =
{
  CMAP_PARSER_CMP_LOOP(SET) cmp_unique, cmp_unique_not,
  init, or_simple, or, and_simple, and
};
