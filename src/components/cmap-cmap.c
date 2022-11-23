
#include "cmap-cmap.h"

#include <cmap/cmap.h>
#define __CMAP_COMMON_H__
#include <cmap/define-min.h>
#include <cmap/aisle.h>
#include "cmap-prototype-map.h"
#include "cmap-prototype-list.h"
#include "cmap-prototype-fn.h"
#include "cmap-prototype-string.h"
#include "cmap-prototype-int.h"
#include "cmap-prototype-double.h"
#include "cmap-prototype-ptr.h"
#include "cmap-console.h"
#include "cmap-scheduler.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create_prototype(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * prototype = $MAP(proc_ctx, CMAP_AISLE_GLOBAL);
  $SET(prototype, "map", cmap_prototype_map_public.instance(proc_ctx));
  $SET(prototype, "list", cmap_prototype_list_public.instance(proc_ctx));
  $SET(prototype, "fn", cmap_prototype_fn_public.instance(proc_ctx));
  $SET(prototype, "string", cmap_prototype_string_public.instance(proc_ctx));
  $SET(prototype, "int", cmap_prototype_int_public.instance(proc_ctx));
  $SET(prototype, "double", cmap_prototype_double_public.instance(proc_ctx));
  $SET(prototype, "ptr", cmap_prototype_ptr_public.instance(proc_ctx));
  return prototype;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * cmap = $MAP(proc_ctx, CMAP_AISLE_GLOBAL);
  $SET(cmap, "prototype", create_prototype(proc_ctx));
  $SET(cmap, "console", cmap_console_public.create(proc_ctx));
  $SET(cmap, "scheduler", cmap_scheduler_public.create(proc_ctx));
  return cmap;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMAP_PUBLIC cmap_cmap_public =
{
  create
};
