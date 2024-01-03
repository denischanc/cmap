
#include "cmap-util.h"

#include <stdlib.h>
#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-log.h"
#include "cmap-string.h"

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

static CMAP_LIST * vto_list(CMAP_PROC_CTX * proc_ctx, va_list maps)
{
  return vfill_list(CMAP_LIST(0, proc_ctx), maps);
}

static CMAP_LIST * to_list(CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list maps;
  va_start(maps, proc_ctx);
  CMAP_LIST * list = vto_list(proc_ctx, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * vto_map(CMAP_PROC_CTX * proc_ctx, va_list key_maps)
{
  CMAP_MAP * ret = CMAP_MAP(proc_ctx);

  const char * key;
  while((key = va_arg(key_maps, char *)) != NULL)
  {
    CMAP_MAP * map = va_arg(key_maps, CMAP_MAP *);
    CMAP_SET(ret, key, map);
  }

  return ret;
}

static CMAP_MAP * to_map(CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list key_maps;
  va_start(key_maps, proc_ctx);
  CMAP_MAP * map = vto_map(proc_ctx, key_maps);
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

static CMAP_LIST * dup_string(CMAP_LIST * dst, CMAP_LIST * src,
  CMAP_PROC_CTX * proc_ctx)
{
  int size = CMAP_CALL(src, size);
  for(int i = 0; i < size; i++)
  {
    CMAP_STRING * string = (CMAP_STRING *)CMAP_LIST_GET(src, i);
    const char * string_val = CMAP_CALL(string, val);
    CMAP_LIST_PUSH(dst, CMAP_STRING(string_val, 0, proc_ctx));
  }
  return dst;
}

/*******************************************************************************
*******************************************************************************/

static char * strdup_(const char * src)
{
  int size = (strlen(src) + 1) * sizeof(char);
  char * dst = (char *)CMAP_KERNEL_MEM -> alloc(size);
  memcpy(dst, src, size);
  return dst;
}

/*******************************************************************************
*******************************************************************************/

static int is_val(CMAP_LIST * list, CMAP_MAP * val)
{
  int max = CMAP_CALL(list, size);
  for(int i = 0; i < max; i++) if(CMAP_LIST_GET(list, i) == val) return i;
  return -1;
}

/*******************************************************************************
*******************************************************************************/

static void inc_w_max(int * i, int max)
{
  if(*i < max - 1) (*i)++;
  else *i = 0;
}

static void dec_w_max(int * i, int max)
{
  if(*i > 0) (*i)--;
  else *i = max - 1;
}

static void add_w_max(int * i, int v, int max)
{
  (*i) += v;
  if(*i < 0) while(*i < 0) (*i) += max;
  else if(*i >= max) while(*i >= max) (*i) -= max;
}

static void rm_w_max(int * i, int v, int max)
{
  (*i) -= v;
  if(*i < 0) while(*i < 0) (*i) += max;
  else if(*i >= max) while(*i >= max) (*i) -= max;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UTIL_PUBLIC cmap_util_public =
{
  fill_list, vfill_list, to_list, vto_list,
  to_map, vto_map,
  uv_error, uv_dummy,
  copy,
  dup_string,
  strdup_,
  is_val,
  inc_w_max, dec_w_max, add_w_max, rm_w_max
};
