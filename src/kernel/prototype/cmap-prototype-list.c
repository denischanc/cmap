
#include "cmap-prototype-list.h"

#include "cmap-aisle.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

static const char * MAIN_FN_NAME = "main";
static const char * APPLY_FN_NAME = "apply";
static const char * ADD_ALL_FN_NAME = "addAll";

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  /* TODO */
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * add_all_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  /* TODO */
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_list_init(CMAP_MAP * proto)
{
  CMAP_SET(proto, APPLY_FN_NAME, CMAP_FN(apply_fn, CMAP_AISLE_KERNEL));
  CMAP_SET(proto, ADD_ALL_FN_NAME, CMAP_FN(add_all_fn, CMAP_AISLE_KERNEL));
}
