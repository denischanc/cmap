
#include "cmap-util-string.h"

#include "cmap-common.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

void cmap_split(CMAP_LIST * list, const char * line, char sep)
{
  int i = 0, off = 0;
  while(CMAP_T)
  {
    if((line[i] == sep) || (line[i] == 0))
    {
      CMAP_STRING * sub = CMAP_STRING("", 0, "TODO");
      CMAP_CALL_ARGS(sub, append_sub, line, off, i);
      CMAP_CALL_ARGS(list, push, (CMAP_MAP *)sub);

      off = i + 1;
    }

    if(line[i] == 0) return;

    i++;
  }
}
