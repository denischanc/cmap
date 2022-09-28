
#include "cmap-util-string.h"

#include "cmap-fw.h"
#include "cmap-common-define.h"
#include "cmap-pool.h"
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

static void cmap_split_w_handler(CMAP_LIST * list, const char * line, char sep,
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

typedef struct
{
  const char * aisle_;
} AISLE_DATA;

static CMAP_STRING * create_handler_from_aisle(void * data)
{
  AISLE_DATA * aisle = (AISLE_DATA *)data;
  return CMAP_STRING("", 0, aisle -> aisle_);
}

static CMAP_STRING * create_handler_from_pool(void * data)
{
  CMAP_POOL_STRING * pool = cmap_kernel() -> fw_.pool_string_;
  return CMAP_CALL(pool, take);
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_split_w_aisle(const char * line, char sep, const char * aisle)
{
  CMAP_LIST * list = CMAP_LIST(0, aisle);
  AISLE_DATA data;
  data.aisle_ = aisle;
  cmap_split_w_handler(list, line, sep, create_handler_from_aisle, &data);
  return list;
}

CMAP_LIST * cmap_split_w_pool(const char * line, char sep)
{
  CMAP_POOL_LIST * pool = cmap_kernel() -> fw_.pool_list_;
  CMAP_LIST * list = CMAP_CALL(pool, take);
  cmap_split_w_handler(list, line, sep, create_handler_from_pool, NULL);
  return list;
}
