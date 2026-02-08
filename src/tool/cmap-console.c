
#include "cmap-console.h"

#include <stdarg.h>
#include <stdio.h>

/*******************************************************************************
*******************************************************************************/

static void info(const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  vfprintf(stdout, txt, args);
  va_end(args);
}

/*******************************************************************************
*******************************************************************************/

static void error(const char * txt, ...)
{
  va_list args;
  va_start(args, txt);
  vfprintf(stderr, txt, args);
  va_end(args);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public = {info, error};
