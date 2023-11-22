
#include "cmap-gen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-part.h"
#include "cmap-option.h"
#include "cmap-string.h"
#include "cmap-gen-main.h"

/*******************************************************************************
*******************************************************************************/

static char only_c = (1 == 0), * this_out_name = NULL, add_main = (1 == 0);

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * out_h_name)
{
  if(!only_c)
  {
    cmap_parser_part_public.add_relative_include(out_h_name);
    cmap_parser_part_public.add_include_lf();
  }
  else cmap_parser_part_public.add_include("cmap-ext.h");
  cmap_parser_part_public.add_include("stdlib.h");
  cmap_parser_part_public.add_include("cmap-int-ext.h");
  cmap_parser_part_public.add_include("cmap-aisle-ext.h");
  cmap_parser_part_public.add_include_lf();
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

  if(add_main) cmap_gen_main_public.impl(cmap_parser_part_public.main());

  fprintf(out, "\n");
  fprintf(out, "%s", cmap_parser_part_public.includes());
  fprintf(out, "%s", *cmap_parser_part_public.functions());
  fprintf(out, "%s", *cmap_parser_part_public.main());

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
    upper_out_name, upper_out_name,
    (cmap_option_public.is_relative_inc()) ?
      "\"cmap-ext.h\"" : "<cmap/cmap-ext.h>",
    *cmap_parser_part_public.headers());

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
      case 'c': only_c = (1 == 1); break;
      case 'f': cmap_option_public.set_fn_name(optarg); break;
      case 'm': add_main = (1 == 1); break;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf(
    "usage: %s gen [cmap file] [c/h root file] (options)\n"
    "options:\n"
    "  -i,--relative-inc                    Relative include\n"
    "  -c,--only-c                          Only c generation\n"
    "  -f,--fn [name]                       Function name\n"
    "  -m,--add-main                        Add main\n",
    this_name);
}

/*******************************************************************************
*******************************************************************************/

static char * create_lower(const char * name)
{
  char * ret = strdup(name);

  for(char * cur = ret; *cur != 0; cur++)
  {
    char c = *cur;
    if((c >= 'A') && (c <= 'Z')) *cur = c + 'a' - 'A';
    else if(((c < 'a') || (c > 'z')) && ((c < '0') || (c > '9'))) *cur = '_';
  }

  return ret;
}

static char * fn_name(const char * base_name)
{
  const char * option_name = cmap_option_public.fn_name();
  if(option_name != NULL) return strdup(option_name);
  else if(base_name != NULL) return create_lower(base_name);
  else if(this_out_name != NULL) return create_lower(this_out_name);
  else return NULL;
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
    this_out_name = out_name;
    cmap_string_public.append_args(&out_c_name, "%s.c", out_name);
    if(!only_c) cmap_string_public.append_args(&out_h_name, "%s.h", out_name);

    add_include(out_h_name);
    if(parse(in_name) != 0) return EXIT_FAILURE;
    if(generate_c(out_c_name) != 0) return EXIT_FAILURE;
    if(!only_c && (generate_h(out_h_name) != 0)) return EXIT_FAILURE;

    cmap_parser_part_public.clean();
    free(out_c_name);
    free(out_h_name);
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_GEN_PUBLIC cmap_gen_public =
{
  fn_name,
  main_
};
