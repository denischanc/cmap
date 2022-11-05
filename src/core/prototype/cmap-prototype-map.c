
#include "cmap-prototype-map.h"

#include <stdlib.h>
#include "cmap-prototype-util.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-common.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_PROTOTYPE_UTIL_MAP_FN map_fn;
  CMAP_STRING * key;
  CMAP_LIST * args;
} MAP_ENTRY_DATA;

static void map_entry_apply_fn(const char * key, CMAP_MAP ** val, void * data)
{
  MAP_ENTRY_DATA * data_ = (MAP_ENTRY_DATA *)data;

  CMAP_STRING * key_ = data_ -> key;
  CMAP_CALL(key_, clear);
  CMAP_CALL_ARGS(key_, append, key);

  CMAP_LIST * args = data_ -> args;
  CMAP_CALL(args, clear);
  CMAP_LIST_PUSH(args, key_);
  CMAP_LIST_PUSH(args, *val);

  CMAP_FN * fn = data_ -> map_fn.fn;
  CMAP_FN_PROC(fn, data_ -> map_fn.map, args);
}

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  MAP_ENTRY_DATA data = {};
  if(cmap_prototype_util_public.args_to_map_fn(args, &data.map_fn))
  {
    CMAP_STRING * key = CMAP_STRING("", 0, NULL);
    CMAP_LIST * args_map_kv = CMAP_LIST(0, NULL);

    data.key = key;
    data.args = args_map_kv;
    CMAP_CALL_ARGS(map, apply, map_entry_apply_fn, &data);

    CMAP_DELETE(key);
    CMAP_DELETE(args_map_kv);
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static void require()
{
  if(proto == NULL) proto = cmap_map_public.create_root(CMAP_AISLE_KERNEL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * instance()
{
  if(proto == NULL)
  {
    require();

    CMAP_PROTO_SET_FN(proto, "apply", apply_fn);
  }
  return proto;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_MAP_PUBLIC cmap_prototype_map_public =
{
  require,
  instance
};
