
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-gen.h"
#include "cmap-prj.h"
#include "cmap-gen-main.h"
#include "config.h"

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf("%s-%s\n", PACKAGE, VERSION);
  printf("usage: %s [gen|gen-main|prj]\n", this_name);
}

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  if(argc > 1)
  {
    if(!strcmp(argv[1], "gen")) return cmap_gen_public.main(argc, argv);
    else if(!strcmp(argv[1], "gen-main"))
      return cmap_gen_main_public.main(argc, argv);
    else if(!strcmp(argv[1], "prj")) return cmap_prj_public.main(argc, argv);
  }

  usage(argv[0]);
  return EXIT_SUCCESS;
}
