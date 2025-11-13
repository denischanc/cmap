
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-build.h"
#include "cmap-build-main.h"
#include "cmap-prj.h"
#include "cmap-compile.h"
#include "cmap-exec.h"
#include "cmap-string.h"
#include "config.h"

/*******************************************************************************
*******************************************************************************/

#define MODULE_LOOP(macro) \
  macro(BUILD, build) \
  macro(BUILD_MAIN, build_main) \
  macro(PRJ, prj) \
  macro(COMPILE, compile) \
  macro(EXEC, exec)

/*******************************************************************************
*******************************************************************************/

#define MODULE_CHOICE(NAME, name) \
  if(choices == NULL) \
    cmap_string_public.append(&choices, CMAP_##NAME##_MODULE_NAME); \
  else cmap_string_public.append_args( \
    &choices, "|%s", CMAP_##NAME##_MODULE_NAME);

static int usage(const char * this_name)
{
  char * choices = NULL;
  MODULE_LOOP(MODULE_CHOICE)

  printf("%s-%s\n", PACKAGE, VERSION);
  printf("usage: %s [%s]\n", this_name, choices);

  free(choices);

  return EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

#define MODULE_MAIN(NAME , name) \
  if(!strcmp(argv[1], CMAP_##NAME##_MODULE_NAME)) \
    return cmap_##name##_public.main(argc, argv);

int main(int argc, char * argv[])
{
  if(argc > 1)
  {
    MODULE_LOOP(MODULE_MAIN)
  }

  return usage(argv[0]);
}
