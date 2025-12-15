
#include "cmap-build.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-build-main.h"
#include "cmap-fn-name.h"
#include "cmap-config.h"
#include "cmap-do-parse.h"
#include "cmap-file-util.h"

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * out_h_name)
{
  if(!cmap_config_public.is_only_c())
  {
    cmap_part_public.add_include(
      cmap_file_util_public.basename(out_h_name), (1 == 1));
    cmap_string_public.append(cmap_part_public.includes(), "\n");
  }
  else cmap_part_public.add_include("cmap-ext.h", (1 == 0));

  cmap_part_public.add_include("stdlib.h", (1 == 0));
}

/*******************************************************************************
*******************************************************************************/

static char generate_c(const char * out_name)
{
  if(!cmap_config_public.is_quiet()) printf("==[[ Generate : _%s_\n", out_name);

  if(cmap_config_public.is_add_main())
    cmap_build_main_public.impl(cmap_part_public.main());

  return cmap_file_util_public.to_file(out_name, "\n%s\n%s",
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
  if(!cmap_config_public.is_quiet()) printf("==[[ Generate : _%s_\n", out_name);

  char * upper_out_name = create_upper(out_name);

  char ret = cmap_file_util_public.to_file(out_name,
    "#ifndef __%s__\n"
    "#define __%s__\n\n"
    "#include %s\n\n"
    "%s\n"
    "#endif\n",
    upper_out_name, upper_out_name, (cmap_config_public.is_relative_inc()) ?
    "\"cmap-ext.h\"" : "<cmap/cmap-ext.h>", *cmap_part_public.headers());

  free(upper_out_name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 3)
  {
    int ids[] = {CMAP_CONFIG_ID_RELATIVE_INC, CMAP_CONFIG_ID_ONLY_C,
      CMAP_CONFIG_ID_FN, CMAP_CONFIG_ID_ADD_MAIN, CMAP_CONFIG_ID_QUIET, 0};
    return cmap_config_public.usage(
      CMAP_BUILD_MODULE_NAME " [cmap file] [c/h root file] %s", ids);
  }

  cmap_fn_name_public.to_config_when_null(argv[1]);

  char * out_name = argv[2], * out_c_name = NULL, * out_h_name = NULL;
  cmap_string_public.append_args(&out_c_name, "%s.c", out_name);
  if(!cmap_config_public.is_only_c())
    cmap_string_public.append_args(&out_h_name, "%s.h", out_name);

  add_include(out_h_name);
  char ok = (1 == 1);
  if(!cmap_do_parse_public.parse(argv[1])) ok = (1 == 0);
  if(ok && !generate_c(out_c_name)) ok = (1 == 0);
  if(ok && !cmap_config_public.is_only_c() && !generate_h(out_h_name))
    ok = (1 == 0);

  cmap_part_public.clean();
  free(out_c_name);
  free(out_h_name);

  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_BUILD_PUBLIC cmap_build_public = {main_};
