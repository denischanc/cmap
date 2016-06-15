
#include "cmap-fw.h"

#include "cmap-kernel.h"
#include "cmap-util-string.h"

/*******************************************************************************
*******************************************************************************/

void cmap_fw_set_split(CMAP_MAP * map, const char * keys, CMAP_MAP * val)
{
  CMAP_STRING * key;

  if(map == NULL) map = cmap_kernel() -> global_env_;

  CMAP_LIST * keys_split = CMAP_LIST(0, "TODO");
  cmap_split(keys_split, keys, '.');

  int i = 0, i_stop = (CMAP_CALL(keys_split, size) - 1);
  for(; i < i_stop; i++)
  {
    if(map == NULL) return;
    else
    {
      key = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
      map = CMAP_GET(map, CMAP_CALL(key, val));
    }
  }

  if(map != NULL)
  {
    key = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
    CMAP_SET(map, CMAP_CALL(key, val), val);
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fw_get_split(CMAP_MAP * map, const char * keys)
{
  if(map == NULL) map = cmap_kernel() -> global_env_;

  CMAP_LIST * keys_split = CMAP_LIST(0, "TODO");
  cmap_split(keys_split, keys, '.');

  CMAP_STRING * key;
  int i = 0, i_stop = CMAP_CALL(keys_split, size);
  for(; i < i_stop; i++)
  {
    if(map == NULL) return NULL;
    else
    {
      key = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
      map = CMAP_GET(map, CMAP_CALL(key, val));
    }
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fw_vproc(CMAP_MAP * map, const char * fn_name, va_list args)
{
  CMAP_LIST * args_list = CMAP_LIST(0, "TODO");
  CMAP_MAP * arg;
  while((arg = va_arg(args, CMAP_MAP *)) != NULL)
  {
    CMAP_CALL_ARGS(args_list, push, arg);
  }

  CMAP_MAP * fn_tmp = CMAP_GET(map, fn_name);
  if((fn_tmp != NULL) && (CMAP_CALL(fn_tmp, nature) == CMAP_FN_NATURE))
  {
    CMAP_FN * fn = (CMAP_FN *)fn_tmp;
    return CMAP_CALL_ARGS(fn, process, map, args_list);
  }
  else return NULL;
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

  if(map == NULL) map = cmap_kernel() -> global_env_;

  CMAP_LIST * keys_split = CMAP_LIST(0, "TODO");
  cmap_split(keys_split, fn_names, '.');

  int i = 0, i_stop = (CMAP_CALL(keys_split, size) - 1);
  for(; i < i_stop; i++)
  {
    if(map == NULL) return NULL;
    else
    {
      fn_name = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);
      map = CMAP_GET(map, CMAP_CALL(fn_name, val));
    }
  }

  if(map == NULL) return NULL;
  else
  {
    fn_name = (CMAP_STRING *)CMAP_CALL(keys_split, unshift);

    va_list args;
    va_start(args, fn_names);
    CMAP_MAP * result = cmap_fw_vproc(map, CMAP_CALL(fn_name, val), args);
    va_end(args);
    return result;
  }
}
