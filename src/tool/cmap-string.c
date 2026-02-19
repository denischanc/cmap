
#include "cmap-string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

void cmap_string_append(char ** src, const char * txt)
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

void cmap_string_vappend_args(char ** src, const char * txt, va_list args)
{
  static char buffer[1048576];
  vsnprintf(buffer, sizeof(buffer), txt, args);

  cmap_string_append(src, buffer);
}

/*******************************************************************************
*******************************************************************************/

void cmap_string_append_args(char ** src, const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  cmap_string_vappend_args(src, txt, args);
  va_end(args);
}

/*******************************************************************************
*******************************************************************************/

void cmap_string_prepend(char ** src, const char * txt)
{
  char * tmp = *src;
  *src = NULL;
  cmap_string_append(src, txt);
  cmap_string_append(src, tmp);
  free(tmp);
}
