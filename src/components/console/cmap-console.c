
#include "cmap-console.h"

#include <stdio.h>
#include "cmap-ext.h"
#include "cmap-aisle-ext.h"

/*******************************************************************************
*******************************************************************************/

static void display(FILE * f, CMAP_LIST * args, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_STRING * line = cmap_string("", 0, proc_ctx, CMAP_AISLE_LOCAL);
  cmap_lproc((CMAP_MAP *)line, "append", proc_ctx, args);
  fprintf(f, "%s\n", cmap_string_val(line));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * info_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stdout, args, proc_ctx);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * error_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stderr, args, proc_ctx);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_to_map(proc_ctx, CMAP_AISLE_GLOBAL,
    "info", cmap_fn(info_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    "error", cmap_fn(error_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public =
{
  create
};
