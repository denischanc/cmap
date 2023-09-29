
#include "cmap-main-usage.h"

#include <stdio.h>
#include "config.h"

/*******************************************************************************
*******************************************************************************/

static void usage(char * argv[])
{
  printf("%s-%s\n", PACKAGE, VERSION);
  printf("usage: %s [gen|prj]\n", argv[0]);
}

/*******************************************************************************
*******************************************************************************/

static void usage_gen(char * argv[])
{
  printf(
    "usage: %s gen [cmap file] [c/h root file] (options)\n"
    "options:\n"
    "  -i,--relative-inc                    Relative include\n",
    argv[0]);
}

/*******************************************************************************
*******************************************************************************/

static void usage_prj(char * argv[])
{
  printf("usage: %s prj [project directory]\n", argv[0]);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_MAIN_USAGE_PUBLIC cmap_main_usage_public =
{
  usage,
  usage_gen,
  usage_prj
};
