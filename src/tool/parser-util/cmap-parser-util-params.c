
#include "cmap-parser-util-params.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-part.h"
#include "cmap-parser-util.h"
#include "cmap-part-keys.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_KEYS * params = NULL;

/*******************************************************************************
*******************************************************************************/

static void clone()
{
  cmap_part_keys_public.add_all(&params, cmap_part_public.get_params());
}

/*******************************************************************************
*******************************************************************************/

static void get_apply(const char * map, const char * name, void * data)
{
  char * map_ = (map == NULL) ? NULL : strdup(map);
  char * map_name = cmap_parser_util_public.path(map_, strdup(name));

  CMAP_PARSER_UTIL_PARAMS_RET * ret = data;
  cmap_string_public.append_args(&ret -> decl, ", CMAP_MAP * %s", map_name);
  cmap_string_public.append_args(&ret -> impl, ", %s", map_name);

  free(map_name);
}

static CMAP_PARSER_UTIL_PARAMS_RET get()
{
  CMAP_PARSER_UTIL_PARAMS_RET ret = {strdup(""), strdup("")};
  cmap_part_keys_public.apply(params, get_apply, &ret);
  cmap_part_keys_public.delete(&params);
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
