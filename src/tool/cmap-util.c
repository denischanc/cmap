
#include "cmap-util.h"

#include <string.h>

/*******************************************************************************
*******************************************************************************/

static int static_contains(const char * name, const char ** names)
{
  if(name == NULL) return -1;

  int i = 0;
  while(*names != NULL)
  {
    if(!strcmp(*names, name)) return i;
    names++; i++;
  }

  return -1;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UTIL_PUBLIC cmap_util_public = {static_contains};
