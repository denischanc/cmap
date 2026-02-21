
#include "cmap-parser-params.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-strings.h"
#include "cmap-part.h"
#include "cmap-parser-this-args.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * params = NULL;

/*******************************************************************************
*******************************************************************************/

void cmap_parser_params_clone()
{
  cmap_strings_add_all(&params, cmap_part_get_params());
}

/*******************************************************************************
*******************************************************************************/

static void get_apply(const char * map, void * data)
{
  const char * type = cmap_parser_this_args_type(map);
  if(type == NULL) type = "CMAP_MAP *";

  CMAP_PARSER_PARAMS_RET * ret = data;
  cmap_string_append_args(&ret -> decl, ", %s %s", type, map);
  cmap_string_append_args(&ret -> impl, ", %s", map);
}

CMAP_PARSER_PARAMS_RET cmap_parser_params_get()
{
  CMAP_PARSER_PARAMS_RET ret = {strdup(""), strdup("")};
  cmap_strings_apply(params, get_apply, &ret);
  cmap_strings_delete(&params);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_params_delete(CMAP_PARSER_PARAMS_RET ret)
{
  free(ret.decl);
  free(ret.impl);
}
