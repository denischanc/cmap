
#include "cmap-prototype-map.h"

#include "cmap-prototype-util.h"
#include "cmap-fw.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_PROTOTYPE_MAP_FN map_fn_;
  CMAP_STRING * key_;
  CMAP_LIST * args_;
} MAP_ENTRY_DATA;

static void map_entry_apply_fn(const char * key, CMAP_MAP ** val, void * data)
{
  MAP_ENTRY_DATA * _data = (MAP_ENTRY_DATA *)data;

  CMAP_STRING * _key = _data -> key_;
  CMAP_CALL(_key, clear);
  CMAP_CALL_ARGS(_key, append, key);

  CMAP_LIST * args = _data -> args_;
  CMAP_CALL(args, clear);
  CMAP_PUSH(args, _key);
  CMAP_PUSH(args, *val);

  CMAP_FN * fn = _data -> map_fn_.fn_;
  CMAP_CALL_ARGS(fn, process, _data -> map_fn_.map_, args);
}

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  MAP_ENTRY_DATA data = {};
  if(cmap_prototype_args_map_fn(&data.map_fn_, args))
  {
    CMAP_STRING * key = CMAP_STRING("", 0, NULL);
    CMAP_LIST * args_map_kv = CMAP_LIST(0, NULL);

    data.key_ = key;
    data.args_ = args_map_kv;
    CMAP_CALL_ARGS(map, apply, map_entry_apply_fn, &data);

    CMAP_DELETE(key);
    CMAP_DELETE(args_map_kv);
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_map_init(CMAP_MAP * proto)
{
  CMAP_SET(proto, CMAP_APPLY_FN_NAME, CMAP_FN(apply_fn, CMAP_AISLE_KERNEL));
}
