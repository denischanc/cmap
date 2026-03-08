
#include "cmap-prototype-map.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-prototype-util.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_PROTOTYPE_UTIL_MAP_FN map_fn;
  CMAP_STRING * key;
  CMAP_LIST * args;
  CMAP_PROC_CTX * proc_ctx;
} MAP_ENTRY_DATA;

static void map_entry_apply(const char * key, CMAP_MAP ** val, void * data)
{
  MAP_ENTRY_DATA * data_ = (MAP_ENTRY_DATA *)data;

  CMAP_STRING * key_ = data_ -> key;
  cmap_string_clean(key_);
  cmap_string_append(key_, key);

  CMAP_LIST * args = data_ -> args;
  cmap_list_clean(args);
  CMAP_LIST_PUSH(args, key_);
  CMAP_LIST_PUSH(args, *val);

  CMAP_FN * fn = data_ -> map_fn.fn;
  CMAP_FN_PROC(fn, data_ -> proc_ctx, data_ -> map_fn.map, args);
}

static CMAP_MAP * apply_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  MAP_ENTRY_DATA data = {};
  if(cmap_prototype_util_args_to_map_fn(args, &data.map_fn))
  {
    CMAP_STRING * key = CMAP_STRING("", 0, proc_ctx);
    CMAP_LIST * args_map_kv = CMAP_LIST(0, proc_ctx);

    data.key = key;
    data.args = args_map_kv;
    data.proc_ctx = proc_ctx;
    cmap_map_apply(map, map_entry_apply, &data);
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_map_require(CMAP_MAP **proto, CMAP_PROC_CTX * proc_ctx)
{
  *proto = cmap_map_create_root(proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_map_init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "apply", apply_fn, proc_ctx);
}
