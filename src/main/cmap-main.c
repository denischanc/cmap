
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-main-usage.h"

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * out_h_name)
{
  cmap_parser_string_public.append_args(cmap_parser_part_public.includes(),
    "#include \"%s\"\n\n#include <stdlib.h>\n"
    "#include <cmap/cmap-int-ext.h>\n",
    out_h_name);
}

/*******************************************************************************
*******************************************************************************/

static int parse(const char * in_name)
{
  yyscan_t scanner;
  cmap_parser_lex_init(&scanner);

  FILE * in = fopen(in_name, "r");
  cmap_parser_set_in(in, scanner);

  int ret = cmap_parser_parse(scanner);

  fclose(in);

  cmap_parser_lex_destroy(scanner);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void generate_c(const char * out_name)
{
  FILE * out = fopen(out_name, "w");
  printf("==[[ Generate : _%s_\n", out_name);

  fprintf(out, "\n");

  fprintf(out, "%s", *cmap_parser_part_public.includes());
  free(*cmap_parser_part_public.includes());
  fprintf(out, "\n");

  fprintf(out, "%s", *cmap_parser_part_public.functions());
  free(*cmap_parser_part_public.functions());

  fprintf(out, "%s", *cmap_parser_part_public.main());
  free(*cmap_parser_part_public.main());

  fclose(out);
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

static void generate_h(const char * out_name)
{
  FILE * out = fopen(out_name, "w");
  printf("==[[ Generate : _%s_\n", out_name);

  char * upper_out_name = create_upper(out_name);

  fprintf(out,
    "#ifndef __%s__\n"
    "#define __%s__\n\n"
    "#include <cmap/cmap-ext.h>\n\n"
    "%s\n"
    "#endif\n",
    upper_out_name, upper_out_name, *cmap_parser_part_public.definitions());
  free(*cmap_parser_part_public.definitions());

  free(upper_out_name);

  fclose(out);
}

/*******************************************************************************
*******************************************************************************/

static int main_gen(int argc, char * argv[])
{
  if(argc < 4) cmap_main_usage_public.usage_gen(argv);
  else
  {
    char * in_name = argv[2], * out_name = argv[3];
    static char out_c_name[1000], out_h_name[1000];
    snprintf(out_c_name, sizeof(out_c_name), "%s.c", out_name);
    snprintf(out_h_name, sizeof(out_h_name), "%s.h", out_name);

    add_include(out_h_name);
    if(parse(in_name) != 0) return EXIT_FAILURE;
    generate_c(out_c_name);
    generate_h(out_h_name);
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
