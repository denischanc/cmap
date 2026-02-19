
#include "cmap-console.h"

#include <stdarg.h>
#include <stdio.h>

/*******************************************************************************
*******************************************************************************/

void cmap_console_info(const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  vfprintf(stdout, txt, args);
  va_end(args);
}

/*******************************************************************************
*******************************************************************************/

void cmap_console_error(const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  vfprintf(stderr, txt, args);
  va_end(args);
}
