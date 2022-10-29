
#include "cmap-util.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-map.h"
#include "cmap-pool.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

void delete_list_vals(CMAP_LIST * list)
{
  CMAP_MAP * val;
  while((val = CMAP_LIST_UNSHIFT(list)) != NULL) CMAP_DELETE(val);
}

/*******************************************************************************
*******************************************************************************/

void release_pool_list_n_strings(CMAP_LIST * list)
{
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  CMAP_POOL_STRING * pool_string = kernel -> pool_string;
  CMAP_POOL_LIST * pool_list = kernel -> pool_list;

  CMAP_MAP * val;
  while((val = CMAP_LIST_POP(list)) != NULL)
  {
    if(CMAP_NATURE(val) == cmap_string_public.nature)
    {
      CMAP_CALL_ARGS(pool_string, release, (CMAP_STRING *)val);
    }
  }

  CMAP_CALL_ARGS(pool_list, release, list);
}

/*******************************************************************************
*******************************************************************************/

static void split_w_handler(CMAP_LIST * list, const char * line, char sep,
  CMAP_STRING * (*create)(void * data), void * data)
{
  int i = 0, off = 0;
  while(CMAP_T)
  {
    if((line[i] == sep) || (line[i] == 0))
    {
      CMAP_STRING * sub = create(data);
      CMAP_CALL_ARGS(sub, append_sub, line, off, i);
      CMAP_LIST_PUSH(list, sub);

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
  CMAP_POOL_STRING * pool = cmap_kernel_public.instance() -> pool_string;
  return CMAP_CALL(pool, take);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * split_w_aisle(const char * line, char sep,
  const char * aisle)
{
  CMAP_LIST * list = CMAP_LIST(0, aisle);
  split_w_handler(list, line, sep, create_handler_from_aisle, (void *)aisle);
  return list;
}

static CMAP_LIST * split_w_pool(const char * line, char sep)
{
  CMAP_POOL_LIST * pool = cmap_kernel_public.instance() -> pool_list;
  CMAP_LIST * list = CMAP_CALL(pool, take);
  split_w_handler(list, line, sep, create_handler_from_pool, NULL);
  return list;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UTIL_PUBLIC cmap_util_public =
{
  delete_list_vals,
  release_pool_list_n_strings,
  split_w_aisle,
  split_w_pool
};
