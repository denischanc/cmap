
#include "cmap-build.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-build-main.h"
#include "cmap-fn-name.h"
#include "cmap-config.h"
#include "cmap-cli.h"
#include "cmap-do-parse.h"
#include "cmap-file-util.h"
#include "cmap-usage.h"
#include "cmap-console.h"

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * out_h_name)
{
  if(!cmap_config_is_only_c())
  {
    cmap_part_public.add_include(cmap_file_util_basename(out_h_name), (1 == 1));
    cmap_string_append(cmap_part_public.includes(), "\n");
  }
  else cmap_part_public.add_include("cmap-ext.h", (1 == 0));

  cmap_part_public.add_include("stdlib.h", (1 == 0));
}

/*******************************************************************************
*******************************************************************************/

static char generate_c(const char * out_name)
{
  if(!cmap_config_is_quiet()) cmap_console_info(
    "==[[ %sGenerate%s : %s_%s_%s\n", CMAP_ESC_BBLUE, CMAP_ESC_RST,
    CMAP_ESC_YELLOW, out_name, CMAP_ESC_RST);

  if(cmap_config_is_add_main()) cmap_build_main_impl(cmap_part_public.main());

  return cmap_file_util_to_file(out_name, "\n%s\n%s",
    *cmap_part_public.includes(), *cmap_part_public.main());
}

/*******************************************************************************
*******************************************************************************/

static char * create_upper(const char * name)
{
  char * ret = strdup(name);

  for(char * cur = ret; *cur != 0; cur++)
  {
    char c = *cur;
    if((c >= 'a') && (c <= 'z')) *cur = c + 'A' - 'a';
    else if(((c < 'A') || (c > 'Z')) && ((c < '0') || (c > '9'))) *cur = '_';
  }

  return ret;
}

static char generate_h(const char * out_name)
{
  if(!cmap_config_is_quiet()) cmap_console_info(
    "==[[ %sGenerate%s : %s_%s_%s\n", CMAP_ESC_BBLUE, CMAP_ESC_RST,
    CMAP_ESC_YELLOW, out_name, CMAP_ESC_RST);

  char * upper_out_name = create_upper(out_name);

  char ret = cmap_file_util_to_file(out_name,
    "#ifndef __%s__\n"
    "#define __%s__\n\n"
    "#include %s\n\n"
    "%s\n"
    "#endif\n",
    upper_out_name, upper_out_name, (cmap_config_is_relative_inc()) ?
    "\"cmap-ext.h\"" : "<cmap/cmap-ext.h>", *cmap_part_public.headers());

  free(upper_out_name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

int cmap_build_main(int argc, char * argv[])
{
  if((argc < 3) || cmap_config_is_help())
  {
    int ids[] = {CMAP_CLI_ID_RELATIVE_INC, CMAP_CLI_ID_ONLY_C, CMAP_CLI_ID_FN,
      CMAP_CLI_ID_ADD_MAIN, CMAP_CLI_ID_QUIET, 0};
    return cmap_usage(
      CMAP_BUILD_MODULE_NAME " [cmap file] [c/h root file] %s", ids);
  }

  cmap_fn_name_to_config_when_null(argv[1]);

  char * out_name = argv[2], * out_c_name = NULL, * out_h_name = NULL;
  cmap_string_append_args(&out_c_name, "%s.c", out_name);
  if(!cmap_config_is_only_c())
    cmap_string_append_args(&out_h_name, "%s.h", out_name);

  add_include(out_h_name);
  char ok = (1 == 1);
  if(!cmap_do_parse_public.parse(argv[1])) ok = (1 == 0);
  if(ok && !generate_c(out_c_name)) ok = (1 == 0);
  if(ok && !cmap_config_is_only_c() && !generate_h(out_h_name)) ok = (1 == 0);

  cmap_part_public.clean();
  free(out_c_name);
  free(out_h_name);

  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
