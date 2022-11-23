
#include "cmap-util.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-map.h"
#include "cmap-pool.h"
#include "cmap-common.h"
#include "cmap-fn.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

static void delete_list_vals(CMAP_LIST * list)
{
  while(CMAP_CALL(list, size) != 0)
  {
    CMAP_MAP * val = CMAP_LIST_SHIFT(list);
    if(val != NULL) CMAP_DELETE(val);
  }
}

/*******************************************************************************
*******************************************************************************/

static void release_pool_list_n_strings(CMAP_LIST * list,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_POOL_STRING * pool_string = CMAP_CALL(proc_ctx, pool_string);
  CMAP_POOL_LIST * pool_list = CMAP_CALL(proc_ctx, pool_list);

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

typedef struct
{
  const char * aisle;
  CMAP_PROC_CTX * proc_ctx;
} SPLIT_HANDLER_DATA;

static CMAP_STRING * create_handler_from_aisle(void * data)
{
  SPLIT_HANDLER_DATA * data_ = (SPLIT_HANDLER_DATA *)data;
  return CMAP_STRING("", 0, data_ -> proc_ctx, data_ -> aisle);
}

static CMAP_STRING * create_handler_from_pool(void * data)
{
  SPLIT_HANDLER_DATA * data_ = (SPLIT_HANDLER_DATA *)data;
  CMAP_POOL_STRING * pool = CMAP_CALL(data_ -> proc_ctx, pool_string);
  return CMAP_CALL_ARGS(pool, take, data_ -> proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * split_w_aisle(const char * line, char sep,
  CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_LIST * list = CMAP_LIST(0, proc_ctx, aisle);

  SPLIT_HANDLER_DATA data;
  data.proc_ctx = proc_ctx;
  data.aisle = aisle;
  split_w_handler(list, line, sep, create_handler_from_aisle, &data);

  return list;
}

static CMAP_LIST * split_w_pool(const char * line, char sep,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  CMAP_LIST * list = CMAP_CALL_ARGS(pool, take, proc_ctx);

  SPLIT_HANDLER_DATA data;
  data.proc_ctx = proc_ctx;
  split_w_handler(list, line, sep, create_handler_from_pool, &data);

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

static CMAP_LIST * vto_list(CMAP_PROC_CTX * proc_ctx, const char * aisle,
  va_list maps)
{
  return vfill_list(CMAP_LIST(0, proc_ctx, aisle), maps);
}

static CMAP_LIST * to_list(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...)
{
  va_list maps;
  va_start(maps, aisle);
  CMAP_LIST * list = vto_list(proc_ctx, aisle, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * vto_map(CMAP_PROC_CTX * proc_ctx, const char * aisle,
  va_list key_maps)
{
  CMAP_MAP * ret = CMAP_MAP(proc_ctx, aisle);

  const char * key;
  while((key = va_arg(key_maps, char *)) != NULL)
  {
    CMAP_MAP * map = va_arg(key_maps, CMAP_MAP *);
    CMAP_SET(ret, key, map);
  }

  return ret;
}

static CMAP_MAP * to_map(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...)
{
  va_list key_maps;
  va_start(key_maps, aisle);
  CMAP_MAP * map = vto_map(proc_ctx, aisle, key_maps);
  va_end(key_maps);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static void uv_error(int err)
{
  if(err < 0)
  {
    cmap_log_public.fatal("Libuv : %s", uv_strerror(err));
    CMAP_KERNEL_INSTANCE -> fatal();
  }
}

/*******************************************************************************
*******************************************************************************/

static void uv_dummy(uv_work_t * req)
{
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
  uv_error,
  uv_dummy
};
