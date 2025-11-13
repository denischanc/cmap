
#include "cmap-file-util.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

/*******************************************************************************
*******************************************************************************/

static char to_file(const char * path, const char * txt, ...)
{
  FILE * f = fopen(path, "w");
  if(f == NULL)
  {
    fprintf(stderr, "[%s] %s\n", path, strerror(errno));
    return (1 == 0);
  }

  char ret = (1 == 1);
  va_list args;
  va_start(args, txt);
  if(vfprintf(f, txt, args) < 0) ret = (1 == 0);
  va_end(args);

  fclose(f);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * basename_no_ext(const char * path)
{
  char * tmp = strrchr(path, '/');
  char * bn = strdup((tmp != NULL) ? tmp + 1 : path);

  tmp = strrchr(bn, '.');
  if(tmp != NULL) *tmp = 0;

  return bn;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FILE_UTIL_PUBLIC cmap_file_util_public = {to_file, basename_no_ext};
