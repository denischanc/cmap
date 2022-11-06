
#include "cmap-cmap.h"

#include <cmap/cmap.h>
#include <cmap/define-min.h>
#include <cmap/aisle.h>
#include "cmap-console.h"
#include "cmap-prototype-map.h"
#include "cmap-prototype-list.h"
#include "cmap-prototype-fn.h"
#include "cmap-prototype-string.h"
#include "cmap-prototype-int.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create_prototype()
{
  CMAP_MAP * prototype = $MAP(CMAP_AISLE_GLOBAL);
  $SET(prototype, "map", cmap_prototype_map_public.instance());
  $SET(prototype, "list", cmap_prototype_list_public.instance());
  $SET(prototype, "fn", cmap_prototype_fn_public.instance());
  $SET(prototype, "string", cmap_prototype_string_public.instance());
  $SET(prototype, "int", cmap_prototype_int_public.instance());
  return prototype;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create()
{
  CMAP_MAP * cmap = $MAP(CMAP_AISLE_GLOBAL);
  $SET(cmap, "console", cmap_console_public.create());
  $SET(cmap, "prototype", create_prototype());
  return cmap;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMAP_PUBLIC cmap_cmap_public =
{
  create
};
