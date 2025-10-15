
#include "cmap-fn-name.h"

#include <string.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

#define FN_VAR(from) static char * from##_name = NULL;

CMAP_FN_NAME_LOOP(FN_VAR)

/*******************************************************************************
*******************************************************************************/

#define FN_FREE(from) free(from##_name);

static void clean()
{
  CMAP_FN_NAME_LOOP(FN_FREE)
}

/*******************************************************************************
*******************************************************************************/

#define FN_IF(from) if(from##_name != NULL) return from##_name;

static const char * name()
{
  CMAP_FN_NAME_LOOP(FN_IF)
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

#define FN_FN(from) \
static void from_##from(const char * name) \
{ \
  free(from##_name); \
  from##_name = strdup(name); \
}

CMAP_FN_NAME_LOOP(FN_FN)

/*******************************************************************************
*******************************************************************************/

static char * resolve(const char * path)
{
  char * tmp = strrchr(path, '/');
  char * fn_name = strdup((tmp != NULL) ? tmp + 1 : path);

  tmp = strrchr(fn_name, '.');
  if(tmp != NULL) *tmp = 0;

  for(tmp = fn_name; *tmp != 0; tmp++)
  {
    char c = *tmp;
    if((c >= 'A') && (c <= 'Z')) *tmp = c + 'a' - 'A';
    else if(((c < 'a') || (c > 'z')) && ((c < '0') || (c > '9'))) *tmp = '_';
  }

  return fn_name;
}

static void from_path_resolve(const char * path)
{
  if(path == NULL) return;

  char * fn_name = resolve(path);
  from_path(fn_name);
  free(fn_name);
}

/*******************************************************************************
*******************************************************************************/

#define FN_SET(from) from_##from,

const CMAP_FN_NAME_PUBLIC cmap_fn_name_public =
{
  clean,
  name,
  CMAP_FN_NAME_LOOP(FN_SET)
  resolve, from_path_resolve
};
