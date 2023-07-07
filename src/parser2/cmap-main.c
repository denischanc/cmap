
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-part.h"

/*******************************************************************************
*******************************************************************************/

static void parse(const char * in_name)
{
  yyscan_t scanner;
  cmap_parser_lex_init(&scanner);

  FILE * in = fopen(in_name, "r");
  cmap_parser_set_in(in, scanner);

  cmap_parser_parse(scanner);

  fclose(in);

  cmap_parser_lex_destroy(scanner);
}

/*******************************************************************************
*******************************************************************************/

static void finalize_c(const char * out_name)
{
  char buffer[1000];
  snprintf(buffer, sizeof(buffer), "%s.c", out_name);
  FILE * out = fopen(buffer, "w");

  fprintf(out, "%s", *cmap_parser_part_public.includes());
  free(*cmap_parser_part_public.includes());
  fprintf(out, "\n");

  fprintf(out, "%s", *cmap_parser_part_public.functions());
  free(*cmap_parser_part_public.functions());

  fprintf(out, "%s", *cmap_parser_part_public.main());
  free(*cmap_parser_part_public.main());

  fclose(out);
}

static char * format_out_h(const char * out_name)
{
  char * ret = strdup(out_name);

  for(char * cur = ret; *cur != 0; cur++)
  {
    char c = *cur;
    if((c >= 'a') && (c <= 'z')) *cur = c + 'A' - 'a';
    else if(((c < 'A') || (c > 'Z')) && ((c < '0') || (c > '9'))) *cur = '_';
  }

  return ret;
}

static void finalize_h(const char * out_name)
{
  char buffer[1000];
  snprintf(buffer, sizeof(buffer), "%s.h", out_name);
  FILE * out = fopen(buffer, "w");

  char * formatted_out = format_out_h(out_name);

  fprintf(out,
    "#ifndef __%s_H__\n"
    "#define __%s_H__\n\n"
    "#include <cmap-ext.h>\n\n"
    "CMAP_MAP * TODO(CMAP_PROC_CTX * proc_ctx);\n\n"
    "#endif\n",
    formatted_out, formatted_out);

  free(formatted_out);

  fclose(out);
}

static void finalize(const char * out_name)
{
  finalize_c(out_name);
  finalize_h(out_name);
}

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  char * in_name = argv[1], * out_name = argv[2];

  parse(in_name);
  finalize(out_name);

  return EXIT_SUCCESS;
}
