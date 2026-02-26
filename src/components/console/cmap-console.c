
#include "cmap-console.h"

#include <stdio.h>
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

static void display(FILE * f, CMAP_LIST * args, const char * eol,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_STRING * line = cmap_string("", 0, proc_ctx);
  cmap_lproc((CMAP_MAP *)line, "append", proc_ctx, args);
  fprintf(f, "%s%s", cmap_string_val(line), eol);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_console_info(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stdout, args, "\n", proc_ctx);
  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_console_print(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stdout, args, "", proc_ctx);
  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_console_error(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stderr, args, "\n", proc_ctx);
  return map;
}
