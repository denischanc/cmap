
#include "cmap-parser-string.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*******************************************************************************
*******************************************************************************/

struct CMAP_PARSER_STRING
{
  char * txt;
};

/*******************************************************************************
*******************************************************************************/

CMAP_PARSER_STRING * cmap_parser_string_create(const char * txt)
{
  CMAP_PARSER_STRING * string = (CMAP_PARSER_STRING *)malloc(
    sizeof(CMAP_PARSER_STRING));
  string -> txt = (char *)malloc((strlen(txt) + 1) * sizeof(char));
  string -> txt[0] = 0;
  strcat(string -> txt, txt);
  return string;
}

/*******************************************************************************
*******************************************************************************/

CMAP_PARSER_STRING * cmap_parser_string_create_args(const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  char buffer[1000];
  vsnprintf(buffer, sizeof(buffer), txt, args);
  va_end(args);

  return cmap_parser_string_create(buffer);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_append(CMAP_PARSER_STRING * string,
  const char * txt)
{
  string -> txt = (char *)realloc(string -> txt,
    (strlen(string -> txt) + strlen(txt) + 1) * sizeof(char));
  strcat(string -> txt, txt);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_append_args(CMAP_PARSER_STRING * string,
  const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  char buffer[1000];
  vsnprintf(buffer, sizeof(buffer), txt, args);
  va_end(args);

  cmap_parser_string_append(string, buffer);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_append_string(CMAP_PARSER_STRING * dst,
  CMAP_PARSER_STRING * src)
{
  cmap_parser_string_append(dst, src -> txt);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_prepend(CMAP_PARSER_STRING * string,
  const char * txt)
{
  char * txt_ = string -> txt;
  string -> txt = (char *)malloc(
    (strlen(txt_) + strlen(txt) + 1) * sizeof(char));

  string -> txt[0] = 0;
  strcat(string -> txt, txt);
  strcat(string -> txt, txt_);
  free(txt_);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_prepend_args(CMAP_PARSER_STRING * string,
  const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  char buffer[1000];
  vsnprintf(buffer, sizeof(buffer), txt, args);
  va_end(args);

  cmap_parser_string_prepend(string, buffer);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_fprintf(FILE * f, CMAP_PARSER_STRING * string)
{
  fprintf(f, "%s", string -> txt);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_clear(CMAP_PARSER_STRING * string)
{
  free(string -> txt);
  string -> txt = (char *)malloc(sizeof(char));
  string -> txt[0] = 0;
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_string_delete(CMAP_PARSER_STRING * string)
{
  free(string -> txt);
  free(string);
}

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_string_dupndel(CMAP_PARSER_STRING * string)
{
  char * ret = strdup(string -> txt);
  cmap_parser_string_delete(string);
  return ret;
}
