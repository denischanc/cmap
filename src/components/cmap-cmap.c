
#include "cmap-cmap.h"

#include "cmap-ext.h"
#include "cmap-aisle-ext.h"
#include "cmap-prototype-map.h"
#include "cmap-prototype-list.h"
#include "cmap-prototype-fn.h"
#include "cmap-prototype-string.h"
#include "cmap-prototype-int.h"
#include "cmap-prototype-double.h"
#include "cmap-prototype-ptr.h"
#include "cmap-console.h"
#include "cmap-scheduler.h"
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create_prototype(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_to_map(proc_ctx, CMAP_AISLE_GLOBAL,
    "map", cmap_prototype_map_public.instance(proc_ctx),
    "list", cmap_prototype_list_public.instance(proc_ctx),
    "fn", cmap_prototype_fn_public.instance(proc_ctx),
    "string", cmap_prototype_string_public.instance(proc_ctx),
    "int", cmap_prototype_int_public.instance(proc_ctx),
    "double", cmap_prototype_double_public.instance(proc_ctx),
    "ptr", cmap_prototype_ptr_public.instance(proc_ctx),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_to_map(proc_ctx, CMAP_AISLE_GLOBAL,
    "prototype", create_prototype(proc_ctx),
    "console", cmap_console_public.create(proc_ctx),
    "scheduler", cmap_scheduler_public.create(proc_ctx),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMAP_PUBLIC cmap_cmap_public =
{
  create
};
