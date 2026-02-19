
#include "cmap-util.h"

#include <string.h>

/*******************************************************************************
*******************************************************************************/

int cmap_util_static_contains(const char * name, const char ** names)
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
