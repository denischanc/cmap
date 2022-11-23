
#include "cmap-console.h"

#include <stdio.h>
#include <cmap/cmap.h>
#include <cmap/define-min.h>

/*******************************************************************************
*******************************************************************************/

static void display(FILE * f, CMAP_LIST * args, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_STRING * line = $STR_L("", proc_ctx);
  $$_L(line, "append", proc_ctx, args);
  fprintf(f, "%s\n", $STRV(line));
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
  return $$MAP(proc_ctx, CMAP_AISLE_GLOBAL,
    "info", $FN(info_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    "error", $FN(error_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public =
{
  create
};
