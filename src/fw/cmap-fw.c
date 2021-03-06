
#include "cmap-fw.h"

#include "cmap-kernel.h"
#include "cmap-util-string.h"
#include "cmap-util-pool.h"
#include "cmap-aisle.h"
#include "cmap-util-list.h"

/*******************************************************************************
*******************************************************************************/

void cmap_fw_set_split(CMAP_MAP * map, const char * keys, CMAP_MAP * val)
{
  CMAP_STRING * key;
  CMAP_KERNEL_FW * fw = &(cmap_kernel() -> fw_);
  CMAP_POOL_STRING * pool_string = fw -> pool_string_;

  if(map == NULL) map = fw -> global_env_;

  CMAP_LIST * keys_split = cmap_split_w_pool(keys, '.');

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

  cmap_release_list_n_strings(keys_split);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fw_get_split(CMAP_MAP * map, const char * keys)
{
  CMAP_KERNEL_FW * fw = &(cmap_kernel() -> fw_);
  CMAP_POOL_STRING * pool_string = fw -> pool_string_;

  if(map == NULL) map = fw -> global_env_;

  CMAP_LIST * keys_split = cmap_split_w_pool(keys, '.');

  CMAP_STRING * key;
  int i = 0, i_stop = CMAP_CALL(keys_split, size);
  for(; (i < i_stop) && (map != NULL); i++)
  {
    key = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
    map = CMAP_GET(map, CMAP_CALL(key, val));
    CMAP_CALL_ARGS(pool_string, release, key);
  }

  cmap_release_list_n_strings(keys_split);

  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fw_vproc(CMAP_MAP * map, const char * fn_name, va_list args)
{
  CMAP_LIST * stack_local = CMAP_LIST(0, CMAP_AISLE_STACK);

  CMAP_LIST * args_list = CMAP_LIST(0, CMAP_AISLE_LOCAL);
  CMAP_MAP * arg;
  while((arg = va_arg(args, CMAP_MAP *)) != NULL)
  {
    CMAP_PUSH(args_list, arg);
  }

  CMAP_MAP * ret = NULL;

  CMAP_MAP * fn_tmp = CMAP_GET(map, fn_name);
  if((fn_tmp != NULL) && (CMAP_CALL(fn_tmp, nature) == CMAP_FN_NATURE))
  {
    CMAP_FN * fn = (CMAP_FN *)fn_tmp;
    ret = CMAP_DO_PROCESS(fn, map, args_list);
  }

  cmap_delete_list_vals(stack_local);
  CMAP_WAREHOUSE * wh = cmap_kernel() -> fw_.warehouse_;
  CMAP_CALL_ARGS(wh, delete_last, CMAP_AISLE_STACK);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fw_proc(CMAP_MAP * map, const char * fn_name, ...)
{
  va_list args;
  va_start(args, fn_name);
  CMAP_MAP * result = cmap_fw_vproc(map, fn_name, args);
  va_end(args);
  return result;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fw_proc_split(CMAP_MAP * map, const char * fn_names, ...)
{
  CMAP_STRING * fn_name;
  CMAP_KERNEL_FW * fw = &(cmap_kernel() -> fw_);
  CMAP_POOL_STRING * pool_string = fw -> pool_string_;

  if(map == NULL) map = fw -> global_env_;

  CMAP_LIST * keys_split = cmap_split_w_pool(fn_names, '.');

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
    map = cmap_fw_vproc(map, CMAP_CALL(fn_name, val), args);
    va_end(args);

    CMAP_CALL_ARGS(pool_string, release, fn_name);
  }

  cmap_release_list_n_strings(keys_split);

  return map;
}
