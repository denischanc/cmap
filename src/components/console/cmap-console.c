
#include "cmap-console.h"

#include <stdio.h>
#include "cmap.h"
#include "cmap-aisle.h"

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

static CMAP_MAP * info(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stdout, args, proc_ctx);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * error(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stderr, args, proc_ctx);
  return map;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public = { info, error };
