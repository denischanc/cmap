
#include "cmap-string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*******************************************************************************
*******************************************************************************/

static void append(char ** src, const char * txt)
{
  if(*src == NULL) *src = strdup(txt);
  else
  {
    *src = (char *)realloc(*src,
      (strlen(*src) + strlen(txt) + 1) * sizeof(char));
    strcat(*src, txt);
  }
}

/*******************************************************************************
*******************************************************************************/

static void append_args(char ** src, const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  static char buffer[10000];
  vsnprintf(buffer, sizeof(buffer), txt, args);
  va_end(args);

  append(src, buffer);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STRING_PUBLIC cmap_string_public =
{
  append,
  append_args
};
