
#include "cmap-tool.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-build.h"
#include "cmap-build-main.h"
#include "cmap-prj.h"
#include "cmap-compile.h"
#include "cmap-exec.h"
#include "cmap-usage.h"
#include "cmap-config.h"
#include "cmap-console.h"
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
  macro(COMPILE, compile, "Compile cmap file(s) to produce binary") \
  macro(COMPILE_MODULE, compile, "Compile cmap file(s) to produce module") \
  macro(EXEC, exec, "Execute cmap file(s)")

/*******************************************************************************
*******************************************************************************/

#define MODULE_VAL_DESC(NAME, name, desc) \
  cmap_usage_public.display_val_desc(CMAP_##NAME##_MODULE_NAME, desc);

static void usage()
{
  cmap_console_public.error("%s%s-%s%s\n",
    CMAP_ESC_GREEN, PACKAGE, VERSION, CMAP_ESC_RST);

  int ids[] = {CMAP_CONFIG_ID_VERSION, 0};
  cmap_usage_public.usage("%s [module] ...", ids);

  cmap_console_public.error("%smodules%s:\n", CMAP_ESC_BBLUE, CMAP_ESC_RST);
  MODULE_LOOP(MODULE_VAL_DESC)
}

/*******************************************************************************
*******************************************************************************/

#define MODULE_MAIN(NAME , name, desc) \
  if(!strcmp(argv[0], CMAP_##NAME##_MODULE_NAME)) \
  { \
    module_fnd = (1 == 1); \
    ret = cmap_##name##_public.main(argc, argv); \
  }

int main(int argc, char * argv[])
{
  cmap_tool_name = argv[0];

  cmap_config_public.mng_opts(&argc, &argv);

  int ret = EXIT_FAILURE;
  if(cmap_config_public.is_version())
  {
    cmap_console_public.info("%s\n", VERSION);
    ret = EXIT_SUCCESS;
  }
  else if(argc < 1) usage();
  else
  {
    char module_fnd = (1 == 0);
    MODULE_LOOP(MODULE_MAIN)
    if(!module_fnd) usage();
  }

  cmap_config_public.clean();

  return ret;
}
