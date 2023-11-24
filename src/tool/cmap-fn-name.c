
#include "cmap-fn-name.h"

#include <string.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

#define VAR(from, format) static char * from##_name = NULL;

CMAP_FN_NAME_LOOP(VAR)

/*******************************************************************************
*******************************************************************************/

#define FREE(from, format) free(from##_name);

static void clean()
{
  CMAP_FN_NAME_LOOP(FREE)
}

/*******************************************************************************
*******************************************************************************/

#define IF(from, format) if(from##_name != NULL) return from##_name;

static char * name()
{
  CMAP_FN_NAME_LOOP(IF)
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static void format_(char * name)
{
  for(char * cur = name; *cur != 0; cur++)
  {
    char c = *cur;
    if((c >= 'A') && (c <= 'Z')) *cur = c + 'a' - 'A';
    else if(((c < 'a') || (c > 'z')) && ((c < '0') || (c > '9'))) *cur = '_';
  }
}

#define FN(from, format) \
static void from_##from(const char * name) \
{ \
  from##_name = strdup(name); \
  if(format) format_(from##_name); \
}

CMAP_FN_NAME_LOOP(FN)

/*******************************************************************************
*******************************************************************************/

#define SET(from, format) from_##from,

const CMAP_FN_NAME_PUBLIC cmap_fn_name_public =
{
  clean,
  name,
  CMAP_FN_NAME_LOOP(SET)
};
