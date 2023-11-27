
#include "cmap-file-util.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

/*******************************************************************************
*******************************************************************************/

static int to_file(const char * path, const char * txt)
{
  FILE * f = fopen(path, "w");
  if(f == NULL)
  {
    fprintf(stderr, "[%s] %s\n", path, strerror(errno));
    return errno;
  }

  int ret = 0;
  if(fprintf(f, txt) < 0) ret = 1;

  fclose(f);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FILE_UTIL_PUBLIC cmap_file_util_public = { to_file };
