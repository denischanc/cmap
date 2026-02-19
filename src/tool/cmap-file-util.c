
#include "cmap-file-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "cmap-console.h"

/*******************************************************************************
*******************************************************************************/

char cmap_file_util_to_file(const char * path, const char * txt, ...)
{
  FILE * f = fopen(path, "w");
  if(f == NULL)
  {
    cmap_console_error("[%s] %s\n", path, strerror(errno));
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

char * cmap_file_util_dirname(const char * path)
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

const char * cmap_file_util_basename(const char * path)
{
  const char * tmp = strrchr(path, '/');
  return (tmp == NULL) ? path : tmp + 1;
}

char * cmap_file_util_basename_no_ext(const char * path)
{
  char * bn = strdup(cmap_file_util_basename(path));

  char * tmp = strrchr(bn, '.');
  if(tmp != NULL) *tmp = 0;

  return bn;
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_file_util_extension(const char * path)
{
  char * tmp = strrchr(path, '.');
  return (tmp != NULL) ? tmp + 1 : path + strlen(path);
}
