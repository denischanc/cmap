
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

/*******************************************************************************
*******************************************************************************/

static char relative_inc = (1 == 0), only_c = (1 == 0);

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
    (relative_inc) ? "\"cmap-ext.h\"" : "<cmap/cmap-ext.h>",
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
  {NULL, 0, NULL, 0}
};

static const char * gen_short_options = "ic";

static void mng_options(int argc, char * argv[])
{
  int o;
  while((o = getopt_long(argc, argv, gen_short_options, gen_long_options,
    NULL)) != -1)
  {
    switch(o)
    {
      case 'i': relative_inc = (1 == 1); break;
      case 'c': only_c = (1 == 1); break;
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
    "  -c,--only-c                          Only c generation\n",
    this_name);
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

    char * in_name = argv[2], * out_name = argv[3];
    static char out_c_name[1000], out_h_name[1000];
    snprintf(out_c_name, sizeof(out_c_name), "%s.c", out_name);
    if(!only_c) snprintf(out_h_name, sizeof(out_h_name), "%s.h", out_name);

    add_include(out_h_name);
    if(parse(in_name) != 0) return EXIT_FAILURE;
    if(generate_c(out_c_name) != 0) return EXIT_FAILURE;
    if(!only_c && (generate_h(out_h_name) != 0)) return EXIT_FAILURE;

    cmap_parser_part_public.clean();
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static char relative_inc_()
{
  return relative_inc;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_GEN_PUBLIC cmap_gen_public =
{
  main_,
  relative_inc_
};
