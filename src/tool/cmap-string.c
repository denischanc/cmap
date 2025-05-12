
#include "cmap-string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

static void vappend_args(char ** src, const char * txt, va_list args)
{
  static char buffer[1048576];
  vsnprintf(buffer, sizeof(buffer), txt, args);

  append(src, buffer);
}

/*******************************************************************************
*******************************************************************************/

static void append_args(char ** src, const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  vappend_args(src, txt, args);
  va_end(args);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STRING_PUBLIC cmap_string_public = {append, vappend_args,
  append_args};
