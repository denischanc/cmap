
#include "cmap-parser-string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*******************************************************************************
*******************************************************************************/

static char * create(const char * txt)
{
  return strdup(txt);
}

/*******************************************************************************
*******************************************************************************/

static char * create_args(const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  char buffer[1000];
  vsnprintf(buffer, sizeof(buffer), txt, args);
  va_end(args);

  return create(buffer);
}

/*******************************************************************************
*******************************************************************************/

static void append(char ** src, const char * txt)
{
  *src = (char *)realloc(*src, (strlen(*src) + strlen(txt) + 1) * sizeof(char));
  strcat(*src, txt);
}

/*******************************************************************************
*******************************************************************************/

static void append_args(char ** src, const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  char buffer[1000];
  vsnprintf(buffer, sizeof(buffer), txt, args);
  va_end(args);

  append(src, buffer);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_STRING_PUBLIC cmap_parser_string_public =
{
  create,
  create_args,
  append,
  append_args
};
