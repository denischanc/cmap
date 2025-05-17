
#include "cmap-util.h"

#include <string.h>

/*******************************************************************************
*******************************************************************************/

static char static_contains(const char * name, const char ** names)
{
  if(name == NULL) return (1 == 0);

  while(*names != NULL)
  {
    if(!strcmp(*names, name)) return (1 == 1);
    names++;
  }
  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UTIL_PUBLIC cmap_util_public = {static_contains};
