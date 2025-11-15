
#include "cmap-build.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-build-main.h"
#include "cmap-fn-name.h"
#include "cmap-config.h"
#include "cmap-do-parse.h"

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * out_h_name)
{
  if(!cmap_config_public.is_only_c())
  {
    cmap_part_public.add_include(out_h_name, (1 == 1));
    cmap_string_public.append(cmap_part_public.includes(), "\n");
  }
  else cmap_part_public.add_include("cmap-ext.h", (1 == 0));
  cmap_part_public.add_include("stdlib.h", (1 == 0));
  cmap_part_public.add_include("cmap-int-ext.h", (1 == 0));
  cmap_part_public.add_include("cmap-string-ext.h", (1 == 0));
}

/*******************************************************************************
*******************************************************************************/

static int generate_c(const char * out_name)
{
  FILE * out = fopen(out_name, "w");
  if(out == NULL)
  {
    fprintf(stderr, "[%s] %s\n", out_name, strerror(errno));
    return 1;
  }
  if(!cmap_config_public.is_quiet()) printf("==[[ Generate : _%s_\n", out_name);

  if(cmap_config_public.is_add_main())
    cmap_build_main_public.impl(cmap_part_public.main());

  fprintf(out, "\n%s\n", *cmap_part_public.includes());
  fprintf(out, "%s", *cmap_part_public.main());

  fclose(out);

  return 0;
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

static int generate_h(const char * out_name)
{
  FILE * out = fopen(out_name, "w");
  if(out == NULL)
  {
    fprintf(stderr, "[%s] %s\n", out_name, strerror(errno));
    return 1;
  }
  if(!cmap_config_public.is_quiet()) printf("==[[ Generate : _%s_\n", out_name);

  char * upper_out_name = create_upper(out_name);

  fprintf(out,
    "#ifndef __%s__\n"
    "#define __%s__\n\n"
    "#include %s\n\n"
    "%s\n"
    "#endif\n",
    upper_out_name, upper_out_name, (cmap_config_public.is_relative_inc()) ?
    "\"cmap-ext.h\"" : "<cmap/cmap-ext.h>", *cmap_part_public.headers());

  free(upper_out_name);

  fclose(out);

  return 0;
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  int ids[] = {CMAP_CONFIG_ID_RELATIVE_INC, CMAP_CONFIG_ID_ONLY_C,
    CMAP_CONFIG_ID_FN, CMAP_CONFIG_ID_ADD_MAIN, CMAP_CONFIG_ID_QUIET, 0};
  cmap_config_public.init_n_check(&argc, &argv, 3,
    CMAP_BUILD_MODULE_NAME " [cmap file] [c/h root file]", ids);

  cmap_fn_name_public.resolve_to_config(argv[1]);

  char * out_name = argv[2], * out_c_name = NULL, * out_h_name = NULL;
  cmap_string_public.append_args(&out_c_name, "%s.c", out_name);
  if(!cmap_config_public.is_only_c())
    cmap_string_public.append_args(&out_h_name, "%s.h", out_name);

  add_include(out_h_name);
  char ok = (1 == 1);
  if(!cmap_do_parse_public.parse(argv[1])) ok = (1 == 0);
  if(ok && (generate_c(out_c_name) != 0)) ok = (1 == 0);
  if(ok && !cmap_config_public.is_only_c() && (generate_h(out_h_name) != 0))
    ok = (1 == 0);

  cmap_part_public.clean();
  free(out_c_name);
  free(out_h_name);

  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_BUILD_PUBLIC cmap_build_public = {main_};
