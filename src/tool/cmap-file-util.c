
#include "cmap-file-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "cmap-console.h"

/*******************************************************************************
*******************************************************************************/

static char to_file(const char * path, const char * txt, ...)
{
  FILE * f = fopen(path, "w");
  if(f == NULL)
  {
    cmap_console_public.error("[%s] %s\n", path, strerror(errno));
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

static char * dirname(const char * path)
{
  if(path == NULL) return strdup(".");
  if(!strcmp(path, "/")) return strdup("/");

  char * tmp = strrchr(path, '/');
  if(tmp == NULL) return strdup(".");

  int size = tmp - path;
  char * ret = malloc((size + 1) * sizeof(char));
  strncpy(ret, path, size);
  ret[size] = 0;
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static const char * basename(const char * path)
{
  const char * tmp = strrchr(path, '/');
  return (tmp == NULL) ? path : tmp + 1;
}

static char * basename_no_ext(const char * path)
{
  char * bn = strdup(basename(path));

  char * tmp = strrchr(bn, '.');
  if(tmp != NULL) *tmp = 0;

  return bn;
}

/*******************************************************************************
*******************************************************************************/

static const char * extension(const char * path)
{
  char * tmp = strrchr(path, '.');
  return (tmp != NULL) ? tmp + 1 : path + strlen(path);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FILE_UTIL_PUBLIC cmap_file_util_public =
{
  to_file,
  dirname, basename, basename_no_ext, extension
};
