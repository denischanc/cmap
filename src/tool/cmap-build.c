
#include "cmap-build.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-part.h"
#include "cmap-option.h"
#include "cmap-string.h"
#include "cmap-build-main.h"
#include "cmap-fn-name.h"
#include "cmap-clean.h"

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * out_h_name)
{
  if(!cmap_option_public.is_only_c())
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

static int parse(const char * in_name)
{
  FILE * in = fopen(in_name, "r");
  if(in == NULL)
  {
    fprintf(stderr, "[%s] %s\n", in_name, strerror(errno));
    return 1;
  }

  yyscan_t scanner;
  cmap_parser_lex_init(&scanner);
  cmap_parser_set_in(in, scanner);

  int ret = cmap_parser_parse(scanner);

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
  printf("==[[ Generate : _%s_\n", out_name);

  if(cmap_option_public.is_add_main())
    cmap_build_main_public.impl(cmap_part_public.main());

  fprintf(out, "\n%s\n", *cmap_part_public.includes());
  fprintf(out, "%s", *cmap_part_public.functions());
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
  printf("==[[ Generate : _%s_\n", out_name);

  char * upper_out_name = create_upper(out_name);

  fprintf(out,
    "#ifndef __%s__\n"
    "#define __%s__\n\n"
    "#include %s\n\n"
    "%s\n"
    "#endif\n",
    upper_out_name, upper_out_name, (cmap_option_public.is_relative_inc()) ?
    "\"cmap-ext.h\"" : "<cmap/cmap-ext.h>", *cmap_part_public.headers());

  free(upper_out_name);

  fclose(out);

  return 0;
}

/*******************************************************************************
*******************************************************************************/

static struct option gen_long_options[] =
{
  {"relative-inc", no_argument, NULL, 'i'},
  {"only-c", no_argument, NULL, 'c'},
  {"fn", required_argument, NULL, 'f'},
  {"add-main", no_argument, NULL, 'm'},
  {NULL, 0, NULL, 0}
};

static const char * gen_short_options = "icf:m";

static void mng_options(int argc, char * argv[])
{
  int o;
  while((o = getopt_long(argc, argv, gen_short_options, gen_long_options,
    NULL)) != -1)
  {
    switch(o)
    {
      case 'i': cmap_option_public.relative_inc(); break;
      case 'c': cmap_option_public.only_c(); break;
      case 'f': cmap_fn_name_public.from_option(optarg); break;
      case 'm': cmap_option_public.add_main(); break;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf(
    "usage: %s %s [cmap file] [c/h root file] (options)\n"
    "options:\n"
    "  -i,--relative-inc                    Relative include\n"
    "  -c,--only-c                          Only c generation\n"
    "  -f,--fn [name]                       Function name\n"
    "  -m,--add-main                        Add main\n",
    this_name, CMAP_BUILD_MODULE_NAME);
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 4) usage(argv[0]);
  else
  {
    optind = 4;
    mng_options(argc, argv);

    char * in_name = argv[2], * out_name = argv[3],
      * out_c_name = NULL, * out_h_name = NULL;
    cmap_fn_name_public.from_basename_no_suffix(out_name);
    cmap_string_public.append_args(&out_c_name, "%s.c", out_name);
    if(!cmap_option_public.is_only_c())
      cmap_string_public.append_args(&out_h_name, "%s.h", out_name);

    add_include(out_h_name);
    if(parse(in_name) != 0) return EXIT_FAILURE;
    if(generate_c(out_c_name) != 0) return EXIT_FAILURE;
    if(!cmap_option_public.is_only_c() && (generate_h(out_h_name) != 0))
      return EXIT_FAILURE;

    cmap_clean_public.clean();
    free(out_c_name);
    free(out_h_name);
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_BUILD_PUBLIC cmap_build_public = { main_ };
