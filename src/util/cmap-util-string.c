
#include "cmap-util-string.h"

#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

void cmap_split_handler(CMAP_LIST * list, const char * line, char sep,
  CMAP_STRING * (*create)(void * data), void * data)
{
  int i = 0, off = 0;
  while(CMAP_T)
  {
    if((line[i] == sep) || (line[i] == 0))
    {
      CMAP_STRING * sub = create(data);
      CMAP_CALL_ARGS(sub, append_sub, line, off, i);
      CMAP_PUSH(list, sub);

      if(line[i] == 0) return;

      off = i + 1;
    }

    i++;
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRING * create_handler_from_aisle(void * data)
{
  const char * aisle = (const char *)data;
  return CMAP_STRING("", 0, aisle);
}

static CMAP_STRING * create_handler_from_pool(void * data)
{
  /* TO CONTINUE */
  return NULL;
}

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
      CMAP_PUSH(list, sub);

      if(line[i] == 0) return;

      off = i + 1;
    }

    i++;
  }
}
