
#include "cmap-parser-util-params.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-strings.h"
#include "cmap-part.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * params = NULL;

/*******************************************************************************
*******************************************************************************/

static void clone()
{
  cmap_strings_public.add_all(&params, cmap_part_public.get_params());
}

/*******************************************************************************
*******************************************************************************/

static void get_apply(const char * map, void * data)
{
  CMAP_PARSER_UTIL_PARAMS_RET * ret = data;
  cmap_string_public.append_args(&ret -> decl, ", CMAP_MAP * %s", map);
  cmap_string_public.append_args(&ret -> impl, ", %s", map);
}

static CMAP_PARSER_UTIL_PARAMS_RET get()
{
  CMAP_PARSER_UTIL_PARAMS_RET ret = {strdup(""), strdup("")};
  cmap_strings_public.apply(params, get_apply, &ret);
  cmap_strings_public.delete(&params);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PARSER_UTIL_PARAMS_RET ret)
{
  free(ret.decl);
  free(ret.impl);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_UTIL_PARAMS_PUBLIC cmap_parser_util_params_public =
  {clone, get, delete};
