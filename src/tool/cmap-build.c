
#include "cmap-build.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-build-main.h"
#include "cmap-fn-name.h"
#include "cmap-clean.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

static char * parse_path = NULL;

/*******************************************************************************
*******************************************************************************/

static void set_parse_path(const char * path)
{
  free(parse_path);
  parse_path = (path != NULL) ? strdup(path) : NULL;
}

static const char * get_parse_path()
{
  return parse_path;
}

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

static int parse()
{
  FILE * in = fopen(parse_path, "r");
  if(in == NULL)
  {
    fprintf(stderr, "[%s] %s\n", parse_path, strerror(errno));
    return 1;
  }

  yyscan_t scanner;
  cmap_parser_lex_init(&scanner);
  cmap_parser_set_in(in, scanner);

  int ret = cmap_parser_parse(scanner);
  cmap_part_public.ctx.clean();

  cmap_parser_lex_destroy(scanner);

  fclose(in);

  return ret;
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

  set_parse_path(argv[1]);
  cmap_fn_name_public.resolve_to_config(parse_path);

  char * out_name = argv[2], * out_c_name = NULL, * out_h_name = NULL;
  cmap_string_public.append_args(&out_c_name, "%s.c", out_name);
  if(!cmap_config_public.is_only_c())
    cmap_string_public.append_args(&out_h_name, "%s.h", out_name);

  add_include(out_h_name);
  char ok = (1 == 1);
  if(parse() != 0) ok = (1 == 0);
  if(ok && (generate_c(out_c_name) != 0)) ok = (1 == 0);
  if(ok && !cmap_config_public.is_only_c() && (generate_h(out_h_name) != 0))
    ok = (1 == 0);

  cmap_clean_public.clean();
  free(out_c_name);
  free(out_h_name);
  set_parse_path(NULL);

  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_BUILD_PUBLIC cmap_build_public =
{
  set_parse_path, get_parse_path, parse,
  main_
};
