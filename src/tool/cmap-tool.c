
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-build.h"
#include "cmap-build-main.h"
#include "cmap-prj.h"
#include "cmap-compile.h"
#include "cmap-exec.h"
#include "cmap-string.h"
#include "cmap-usage.h"
#include "config.h"

/*******************************************************************************
*******************************************************************************/

#define MODULE_LOOP(macro) \
  macro(BUILD, build, "Build c/h file from cmap file") \
  macro(BUILD_MAIN, build_main, \
    "Build c file with main function to call kernel") \
  macro(PRJ, prj, "Create examples projects") \
  macro(COMPILE, compile, "Compile cmap file(s) to produce executable") \
  macro(EXEC, exec, "Execute cmap file(s)")

/*******************************************************************************
*******************************************************************************/

#define MODULE_VAL_DESC(NAME, name, desc) \
  cmap_usage_public.print_val_desc(CMAP_##NAME##_MODULE_NAME, desc);

static int usage(const char * this_name)
{
  printf("%s-%s\n", PACKAGE, VERSION);
  printf("usage: %s [module] ...\nmodules:\n", this_name);
  MODULE_LOOP(MODULE_VAL_DESC)

  return EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

#define MODULE_MAIN(NAME , name, desc) \
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
