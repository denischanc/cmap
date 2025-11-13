
#include "cmap-fn-name.h"

#include <stdlib.h>
#include "cmap-file-util.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

static char * resolve(const char * path)
{
  char * fn_name = cmap_file_util_public.basename_no_ext(path);

  for(char * tmp = fn_name; *tmp != 0; tmp++)
  {
    char c = *tmp;
    if((c >= 'A') && (c <= 'Z')) *tmp = c + 'a' - 'A';
    else if(((c < 'a') || (c > 'z')) && ((c < '0') || (c > '9'))) *tmp = '_';
  }

  return fn_name;
}

/*******************************************************************************
*******************************************************************************/

static void resolve_to_config(const char * path)
{
  if((path != NULL) && (cmap_config_public.fn() == NULL))
  {
    char * fn_name = resolve(path);
    cmap_config_public.set_fn(fn_name);
    free(fn_name);
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FN_NAME_PUBLIC cmap_fn_name_public = {resolve, resolve_to_config};
