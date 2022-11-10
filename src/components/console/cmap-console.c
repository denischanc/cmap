
#include "cmap-console.h"

#include <stdio.h>
#include <cmap/cmap.h>
#include <cmap/define-min.h>

/*******************************************************************************
*******************************************************************************/

static void display(FILE * f, CMAP_LIST * args)
{
  CMAP_STRING * line = $STR_L("");
  $$_L(line, "append", args);
  fprintf(f, "%s\n", $STRV(line));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * info_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stdout, args);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * error_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  display(stderr, args);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create()
{
  return $$MAP(CMAP_AISLE_GLOBAL,
    "info", $FN(info_fn, CMAP_AISLE_GLOBAL),
    "error", $FN(error_fn, CMAP_AISLE_GLOBAL),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public =
{
  create
};
