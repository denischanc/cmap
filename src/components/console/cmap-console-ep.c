
#include "cmap-console-ep.h"

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

CMAP_MAP * cmap_console_ep_public_info(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  display(stdout, args, proc_ctx);
  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_console_ep_public_error(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  display(stderr, args, proc_ctx);
  return map;
}
