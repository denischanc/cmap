
#include "cmap-prj.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "cmap-file-util.h"
#include "cmap-string.h"
#include "config.h"
#include "cmap-prj-simple-hello-world-cmap.h"
#include "cmap-prj-simple-makefile.h"

/*******************************************************************************
*******************************************************************************/

static int to_file(const char * dir, const char * file_name, const char * txt)
{
  char * path = NULL;
  cmap_string_public.append_args(&path, "%s/%s", dir, file_name);
  int ret = cmap_file_util_public.to_file(path, txt);
  free(path);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf("usage: %s %s [project directory]\n", this_name, CMAP_PRJ_MODULE_NAME);
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 3) usage(argv[0]);
  else
  {
    char * dir = argv[2];
    if(mkdir(dir, 0755) < 0)
    {
      fprintf(stderr, "[%s] %s\n", dir, strerror(errno));
      return EXIT_FAILURE;
    }

    if(to_file(dir, "hello-world.cmap", CMAP_PRJ_SIMPLE_HELLO_WORLD_CMAP) != 0)
      return EXIT_FAILURE;
    if(to_file(dir, "Makefile", CMAP_PRJ_SIMPLE_MAKEFILE) != 0)
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PRJ_PUBLIC cmap_prj_public = { main_ };
