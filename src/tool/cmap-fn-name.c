
#include "cmap-fn-name.h"

#include <stdlib.h>
#include "cmap-file-util.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

char * cmap_fn_name_resolve(const char * path)
{
  char * fn_name = cmap_file_util_basename_no_ext(path);

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

void cmap_fn_name_to_config(const char * path)
{
  if(path == NULL) return;

  char * fn_name = cmap_fn_name_resolve(path);
  cmap_config_set_fn(fn_name);
  free(fn_name);
}

void cmap_fn_name_to_config_when_null(const char * path)
{
  if(cmap_config_fn() == NULL) cmap_fn_name_to_config(path);
}
