
#include "cmap-util.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-log.h"

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

static void copy_apply_fn(const char * key, CMAP_MAP ** val, void * data)
{
  CMAP_MAP * dst = (CMAP_MAP *)data;
  if(dst != NULL) CMAP_SET(dst, key, *val);
}

static CMAP_MAP * copy(CMAP_MAP * dst, CMAP_MAP * src)
{
  if(src != NULL) CMAP_CALL_ARGS(src, apply, copy_apply_fn, dst);
  return dst;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UTIL_PUBLIC cmap_util_public =
{
  delete_list_vals,
  fill_list,
  vfill_list,
  to_list,
  vto_list,
  to_map,
  vto_map,
  uv_error,
  uv_dummy,
  copy
};
