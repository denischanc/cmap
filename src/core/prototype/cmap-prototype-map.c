
#include "cmap-prototype-map.h"

#include <stdlib.h>
#include "cmap-prototype-util.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-common.h"
#include "cmap-aisle.h"
#include "cmap-map.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;
static char proto_ok = CMAP_F;

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
  return map;
}

/*******************************************************************************
*******************************************************************************/

static void delete_apply_fn(const char * key, CMAP_MAP ** val, void * data)
{
  if(!CMAP_CALL(*val, is_ref))
  {
    CMAP_CALL_ARGS(*val, apply, delete_apply_fn, data);
    CMAP_DELETE(*val);
  }
}

static CMAP_MAP * deep_delete_no_ref(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  delete_apply_fn(NULL, &map, NULL);
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * require()
{
  if(proto == NULL) proto = cmap_map_public.create_root(CMAP_AISLE_KERNEL);
  return proto;
}

/*******************************************************************************
*******************************************************************************/

static void init()
{
  CMAP_PROTO_SET_FN(proto, "apply", apply_fn);
  CMAP_PROTO_SET_FN(proto, "deepDeleteNoRef", deep_delete_no_ref);
}

static CMAP_MAP * instance()
{
  return cmap_prototype_util_public.instance(&proto, &proto_ok, require, init);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_MAP_PUBLIC cmap_prototype_map_public =
{
  require,
  instance
};
