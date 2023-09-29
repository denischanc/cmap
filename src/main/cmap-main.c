
#include "cmap-main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-part.h"
#include "cmap-string.h"
#include "cmap-main-usage.h"

/*******************************************************************************
*******************************************************************************/

char relative_inc = (0 == 1);

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * out_h_name)
{
  cmap_parser_part_public.add_relative_include(out_h_name);
  cmap_parser_part_public.add_include_lf();
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
  free(cmap_parser_part_public.includes());

  fprintf(out, "%s", *cmap_parser_part_public.c_impl());
  free(*cmap_parser_part_public.c_impl());

  fprintf(out, "%s", *cmap_parser_part_public.functions());
  free(*cmap_parser_part_public.functions());

  fprintf(out, "%s", *cmap_parser_part_public.main());
  free(*cmap_parser_part_public.main());

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
  free(*cmap_parser_part_public.headers());

  free(upper_out_name);

  fclose(out);

  return 0;
}

/*******************************************************************************
*******************************************************************************/

static struct option gen_long_options[] =
{
  {"relative-inc", no_argument, NULL, 'i'},
  {NULL, 0, NULL, 0}
};

static const char * gen_short_options = "i";

static void main_gen_options(int argc, char * argv[])
{
  int o;
  while((o = getopt_long(argc, argv, gen_short_options, gen_long_options,
    NULL)) != -1)
  {
    switch(o)
    {
      case 'i': relative_inc = (1 == 1); break;
    }
  }
}

static int main_gen(int argc, char * argv[])
{
  if(argc < 4) cmap_main_usage_public.usage_gen(argv);
  else
  {
    optind = 4;
    main_gen_options(argc, argv);

    char * in_name = argv[2], * out_name = argv[3];
    static char out_c_name[1000], out_h_name[1000];
    snprintf(out_c_name, sizeof(out_c_name), "%s.c", out_name);
    snprintf(out_h_name, sizeof(out_h_name), "%s.h", out_name);

    add_include(out_h_name);
    if(parse(in_name) != 0) return EXIT_FAILURE;
    if(generate_c(out_c_name) != 0) return EXIT_FAILURE;
    if(generate_h(out_h_name) != 0) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static int main_prj(int argc, char * argv[])
{
  cmap_main_usage_public.usage_prj(argv);
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  if(argc > 1)
  {
    if(!strcmp(argv[1], "gen")) return main_gen(argc, argv);
    else if(!strcmp(argv[1], "prj")) return main_prj(argc, argv);
  }

  cmap_main_usage_public.usage(argv);
  return EXIT_SUCCESS;
}
