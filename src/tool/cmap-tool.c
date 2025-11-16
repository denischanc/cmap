
#include "cmap-tool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-build.h"
#include "cmap-build-main.h"
#include "cmap-prj.h"
#include "cmap-compile.h"
#include "cmap-exec.h"
#include "cmap-usage.h"
#include "cmap-config.h"
#include "config.h"

/*******************************************************************************
*******************************************************************************/

const char * cmap_tool_name;

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

static int usage()
{
  printf("%s-%s\n", PACKAGE, VERSION);
  printf("usage: %s [module] ...\nmodules:\n", cmap_tool_name);
  MODULE_LOOP(MODULE_VAL_DESC)

  return EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

#define MODULE_MAIN(NAME , name, desc) \
  if(!strcmp(argv[0], CMAP_##NAME##_MODULE_NAME)) \
    ret = cmap_##name##_public.main(argc, argv);

int main(int argc, char * argv[])
{
  cmap_tool_name = argv[0];

  cmap_config_public.mng_opts(&argc, &argv);

  if(argc < 1) return usage();

  int ret = EXIT_FAILURE;
  MODULE_LOOP(MODULE_MAIN)

  cmap_config_public.clean();
  return ret;
}
