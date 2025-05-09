
#include "cmap-params.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-strings.h"
#include "cmap-string.h"
#include "cmap-part.h"
#include "cmap-parser-util.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * params = NULL;

/*******************************************************************************
*******************************************************************************/

static void clone()
{
  params = cmap_strings_public.clone(cmap_part_public.get_params());
}

/*******************************************************************************
*******************************************************************************/

static void get_apply(const char * string, void * data)
{
  char * map = cmap_parser_util_public.name(strdup(string));

  PARAMS_RET * ret = data;
  cmap_string_public.append_args(&ret -> decl, ", CMAP_MAP * %s", map);
  cmap_string_public.append_args(&ret -> impl, ", %s", map);

  free(map);
}

static PARAMS_RET get()
{
  PARAMS_RET ret = {strdup(""), strdup("")};
  cmap_strings_public.apply(params, get_apply, &ret);
  cmap_strings_public.delete(&params);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void delete(PARAMS_RET ret)
{
  free(ret.decl);
  free(ret.impl);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARAMS_PUBLIC cmap_params_public = {clone, get, delete};
