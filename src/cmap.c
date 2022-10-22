
#include "cmap.h"

#include <stdlib.h>
#include "cmap-common-define.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-fn.h"
#include "cmap-string.h"
#include "cmap-int.h"
#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-aisle.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

void cmap_init(CMAP_KERNEL_CFG * cfg)
{
  cmap_kernel_public.init(cfg);
}

int cmap_main(int argc, char * argv[])
{
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  if(kernel == NULL) cmap_fatal();
  return kernel -> main(argc, argv);
}

void cmap_exit(int ret)
{
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  if(kernel != NULL) kernel -> exit(ret);
  else exit(ret);
}

void cmap_fatal()
{
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  if(kernel != NULL) kernel -> fatal();
  else exit(EXIT_FAILURE);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map(const char * aisle)
{
  return cmap_map_public.create(aisle);
}

CMAP_LIST * cmap_list(int size_inc, const char * aisle)
{
  return cmap_list_public.create(size_inc, aisle);
}

CMAP_FN * cmap_fn(CMAP_FN_TPL process, const char * aisle)
{
  return cmap_fn_public.create(process, aisle);
}

CMAP_STRING * cmap_string(const char * val, int size_inc, const char * aisle)
{
  return cmap_string_public.create(val, size_inc, aisle);
}

CMAP_INT * cmap_int(const char * aisle)
{
  return cmap_int_public.create(aisle);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_nature(CMAP_MAP * map)
{
  return CMAP_CALL(map, nature);
}

CMAP_MAP * cmap_delete(CMAP_MAP * map)
{
  return CMAP_CALL(map, delete);
}

void cmap_set(CMAP_MAP * map, const char * key, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(map, set, key, val);
}

CMAP_MAP * cmap_get(CMAP_MAP * map, const char * key)
{
  return CMAP_CALL_ARGS(map, get, key);
}

/*******************************************************************************
*******************************************************************************/

void cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(list, set, i, val);
}

CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i)
{
  return CMAP_CALL_ARGS(list, get, i);
}

/*******************************************************************************
*******************************************************************************/

void cmap_set_split(CMAP_MAP * map, const char * keys, CMAP_MAP * val)
{
  CMAP_STRING * key;
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  CMAP_POOL_STRING * pool_string = kernel -> pool_string;

  if(map == NULL) map = kernel -> global_env;

  CMAP_LIST * keys_split = cmap_util_public.split_w_pool(keys, '.');

  int i = 0, i_stop = (CMAP_CALL(keys_split, size) - 1);
  for(; (i < i_stop) && (map != NULL); i++)
  {
    key = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
    map = CMAP_GET(map, CMAP_CALL(key, val));
    CMAP_CALL_ARGS(pool_string, release, key);
  }

  if(map != NULL)
  {
    key = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
    CMAP_SET(map, CMAP_CALL(key, val), val);
    CMAP_CALL_ARGS(pool_string, release, key);
  }

  cmap_util_public.release_pool_list_n_strings(keys_split);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_get_split(CMAP_MAP * map, const char * keys)
{
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  CMAP_POOL_STRING * pool_string = kernel -> pool_string;

  if(map == NULL) map = kernel -> global_env;

  CMAP_LIST * keys_split = cmap_util_public.split_w_pool(keys, '.');

  CMAP_STRING * key;
  int i = 0, i_stop = CMAP_CALL(keys_split, size);
  for(; (i < i_stop) && (map != NULL); i++)
  {
    key = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
    map = CMAP_GET(map, CMAP_CALL(key, val));
    CMAP_CALL_ARGS(pool_string, release, key);
  }

  cmap_util_public.release_pool_list_n_strings(keys_split);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * cmap_vproc(CMAP_MAP * map, const char * fn_name,
  va_list args)
{
  CMAP_LIST * stack_local = CMAP_LIST(0, cmap_aisle_public.stack);

  CMAP_LIST * args_list = CMAP_LIST(0, cmap_aisle_public.local);
  CMAP_MAP * arg;
  while((arg = va_arg(args, CMAP_MAP *)) != NULL)
  {
    CMAP_LIST_PUSH(args_list, arg);
  }

  CMAP_MAP * ret = NULL;

  CMAP_MAP * fn_tmp = CMAP_GET(map, fn_name);
  if((fn_tmp != NULL) && (CMAP_CALL(fn_tmp, nature) == cmap_fn_public.nature))
  {
    CMAP_FN * fn = (CMAP_FN *)fn_tmp;
    ret = CMAP_FN_PROCESS(fn, map, args_list);
  }

  cmap_util_public.delete_list_vals(stack_local);
  CMAP_WAREHOUSE * wh = cmap_kernel_public.instance() -> warehouse;
  CMAP_CALL_ARGS(wh, delete_last, cmap_aisle_public.stack);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_proc(CMAP_MAP * map, const char * fn_name, ...)
{
  va_list args;
  va_start(args, fn_name);
  CMAP_MAP * result = cmap_vproc(map, fn_name, args);
  va_end(args);
  return result;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_proc_split(CMAP_MAP * map, const char * fn_names, ...)
{
  CMAP_STRING * fn_name;
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  CMAP_POOL_STRING * pool_string = kernel -> pool_string;

  if(map == NULL) map = kernel -> global_env;

  CMAP_LIST * keys_split = cmap_util_public.split_w_pool(fn_names, '.');

  int i = 0, i_stop = (CMAP_CALL(keys_split, size) - 1);
  for(; (i < i_stop) && (map != NULL); i++)
  {
    fn_name = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
    map = CMAP_GET(map, CMAP_CALL(fn_name, val));
    CMAP_CALL_ARGS(pool_string, release, fn_name);
  }

  if(map != NULL)
  {
    fn_name = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);

    va_list args;
    va_start(args, fn_names);
    map = cmap_vproc(map, CMAP_CALL(fn_name, val), args);
    va_end(args);

    CMAP_CALL_ARGS(pool_string, release, fn_name);
  }

  cmap_util_public.release_pool_list_n_strings(keys_split);

  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MEM_STATE * cmap_mem_state()
{
  return cmap_mem_public.state();
}
