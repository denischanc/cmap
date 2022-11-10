
#include "cmap-util.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-map.h"
#include "cmap-pool.h"
#include "cmap-common.h"
#include "cmap-fn.h"
#include <uv.h>

/*******************************************************************************
*******************************************************************************/

static void delete_list_vals(CMAP_LIST * list)
{
  CMAP_MAP * val;
  while((val = CMAP_LIST_UNSHIFT(list)) != NULL) CMAP_DELETE(val);
}

/*******************************************************************************
*******************************************************************************/

static void release_pool_list_n_strings(CMAP_LIST * list)
{
  CMAP_POOL_STRING * pool_string = cmap_kernel_public.pool_string();
  CMAP_POOL_LIST * pool_list = cmap_kernel_public.pool_list();

  CMAP_MAP * val;
  while((val = CMAP_LIST_POP(list)) != NULL)
  {
    if(CMAP_NATURE(val) == CMAP_NATURE_STRING)
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
  CMAP_POOL_STRING * pool = cmap_kernel_public.pool_string();
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
  CMAP_POOL_LIST * pool = cmap_kernel_public.pool_list();
  CMAP_LIST * list = CMAP_CALL(pool, take);
  split_w_handler(list, line, sep, create_handler_from_pool, NULL);
  return list;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * vfill_list(CMAP_LIST * list, va_list maps)
{
  CMAP_MAP * map;
  while((map = va_arg(maps, CMAP_MAP *)) != NULL) CMAP_LIST_PUSH(list, map);
  return list;
}

static CMAP_LIST * fill_list(CMAP_LIST * list, ...)
{
  va_list maps;
  va_start(maps, list);
  vfill_list(list, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * vto_list(const char * aisle, va_list maps)
{
  return vfill_list(CMAP_LIST(0, aisle), maps);
}

static CMAP_LIST * to_list(const char * aisle, ...)
{
  va_list maps;
  va_start(maps, aisle);
  CMAP_LIST * list = vto_list(aisle, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * vto_map(const char * aisle, va_list key_maps)
{
  CMAP_MAP * ret = CMAP_MAP(aisle);

  const char * key;
  while((key = va_arg(key_maps, char *)) != NULL)
  {
    CMAP_MAP * map = va_arg(key_maps, CMAP_MAP *);
    CMAP_SET(ret, key, map);
  }

  return ret;
}

static CMAP_MAP * to_map(const char * aisle, ...)
{
  va_list key_maps;
  va_start(key_maps, aisle);
  CMAP_MAP * map = vto_map(aisle, key_maps);
  va_end(key_maps);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static void uv_error(int err)
{
  if(err < 0) cmap_log_public.error("Libuv : %s", uv_strerror(err));
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UTIL_PUBLIC cmap_util_public =
{
  delete_list_vals,
  release_pool_list_n_strings,
  split_w_aisle,
  split_w_pool,
  fill_list,
  vfill_list,
  to_list,
  vto_list,
  to_map,
  vto_map,
  uv_error
};
