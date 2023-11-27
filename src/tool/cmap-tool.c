
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-build.h"
#include "cmap-build-main.h"
#include "cmap-prj.h"
#include "config.h"

/*******************************************************************************
*******************************************************************************/

#define CHOICE_MODULES \
  CMAP_BUILD_MODULE_NAME "|" CMAP_BUILD_MAIN_MODULE_NAME \
  "|" CMAP_PRJ_MODULE_NAME

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf("%s-%s\n", PACKAGE, VERSION);
  printf("usage: %s [%s]\n", this_name, CHOICE_MODULES);
}

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  if(argc > 1)
  {
    if(!strcmp(argv[1], CMAP_BUILD_MODULE_NAME))
      return cmap_build_public.main(argc, argv);
    else if(!strcmp(argv[1], CMAP_BUILD_MAIN_MODULE_NAME))
      return cmap_build_main_public.main(argc, argv);
    else if(!strcmp(argv[1], CMAP_PRJ_MODULE_NAME))
      return cmap_prj_public.main(argc, argv);
  }

  usage(argv[0]);
  return EXIT_SUCCESS;
}
