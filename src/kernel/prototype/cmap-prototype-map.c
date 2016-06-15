
#include "cmap-prototype-map.h"

#include "cmap-fw.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

static const char * MAIN_FN_NAME = "main";
static const char * APPLY_FN_NAME = "apply";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_MAP * map_;
  CMAP_FN * fn_;
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

  CMAP_FN * fn = _data -> fn_;
  CMAP_CALL_ARGS(fn, process, _data -> map_, args);
}

static char apply_ana_args(CMAP_LIST * args, MAP_ENTRY_DATA * data)
{
  if(CMAP_CALL(args, size) < 1) return CMAP_F;

  CMAP_MAP * tmp = CMAP_CALL(args, unshift);
  if(CMAP_NATURE(tmp) == CMAP_FN_NATURE)
  {
    data -> fn_ = (CMAP_FN *)tmp;
    return CMAP_T;
  }
  else
  {
    CMAP_MAP * map = tmp;
    tmp = CMAP_GET(map, MAIN_FN_NAME);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_FN_NATURE))
    {
      data -> map_ = map;
      data -> fn_ = (CMAP_FN *)tmp;
      return CMAP_T;
    }
  }

  return CMAP_F;
}

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  MAP_ENTRY_DATA data = {0};
  if(apply_ana_args(args, &data))
  {
    CMAP_STRING * key = CMAP_STRING("", 0, NULL);
    CMAP_LIST * args = CMAP_LIST(0, NULL);

    data.key_ = key;
    data.args_ = args;
    CMAP_CALL_ARGS(map, apply, map_entry_apply_fn, &data);

    CMAP_DELETE(key);
    CMAP_DELETE(args);
  }
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_map_init(CMAP_MAP * proto)
{
  CMAP_SET(proto, APPLY_FN_NAME, CMAP_FN(apply_fn, CMAP_AISLE_KERNEL));
}
