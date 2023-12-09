
#include "cmap-prj.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "cmap-file-util.h"
#include "cmap-string.h"
#include "cmap-prj-simple-hello-world.cmap.h"
#include "cmap-prj-simple-Makefile.h"
#include "cmap-prj-multiple-main.cmap.h"
#include "cmap-prj-multiple-Makefile.h"
#include "cmap-prj-multiple-random.cmap.h"
#include "cmap-prj-multiple-random-fn.c.h"
#include "cmap-prj-multiple-random-fn.h.h"
#include "cmap-prj-multiple-screen.cmap.h"
#include "cmap-prj-multiple-snake.cmap.h"
#include "cmap-option.h"

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

static int build_files(const char * dir, ...)
{
  int ret = 0;

  va_list fn_txts;
  va_start(fn_txts, dir);

  const char * fn, * txt;
  while(((fn = va_arg(fn_txts, const char *)) != NULL) && (ret == 0))
  {
    txt = va_arg(fn_txts, const char *);
    ret = to_file(dir, fn, txt);
  }

  va_end(fn_txts);

  return ret;
}

static int build_files_simple(const char * dir)
{
  return build_files(dir,
    "hello-world.cmap", CMAP_PRJ_SIMPLE_HELLO_WORLD_CMAP,
    "Makefile", CMAP_PRJ_SIMPLE_MAKEFILE,
    NULL);
}

static int build_files_multiple(const char * dir)
{
  return build_files(dir,
    "main.cmap", CMAP_PRJ_MULTIPLE_MAIN_CMAP,
    "Makefile", CMAP_PRJ_MULTIPLE_MAKEFILE,
    "random.cmap", CMAP_PRJ_MULTIPLE_RANDOM_CMAP,
    "random-fn.c", CMAP_PRJ_MULTIPLE_RANDOM_FN_C,
    "random-fn.h", CMAP_PRJ_MULTIPLE_RANDOM_FN_H,
    "screen.cmap", CMAP_PRJ_MULTIPLE_SCREEN_CMAP,
    "snake.cmap", CMAP_PRJ_MULTIPLE_SNAKE_CMAP,
    NULL);
}

/*******************************************************************************
*******************************************************************************/

static struct option gen_long_options[] =
{
  {"multiple", no_argument, NULL, 'm'},
  {NULL, 0, NULL, 0}
};

static const char * gen_short_options = "m";

static void mng_options(int argc, char * argv[])
{
  int o;
  while((o = getopt_long(argc, argv, gen_short_options, gen_long_options,
    NULL)) != -1)
  {
    switch(o)
    {
      case 'm': cmap_option_public.multiple(); break;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf("usage: %s %s [project directory] (options)\n"
    "options:\n"
    "  -m,--multiple                        Multiple\n",
    this_name, CMAP_PRJ_MODULE_NAME);
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 3) usage(argv[0]);
  else
  {
    optind = 3;
    mng_options(argc, argv);

    char * dir = argv[2];
    if(mkdir(dir, 0755) < 0)
    {
      fprintf(stderr, "[%s] %s\n", dir, strerror(errno));
      return EXIT_FAILURE;
    }

    if(cmap_option_public.is_multiple())
    {
      if(build_files_multiple(dir) != 0) return EXIT_FAILURE;
    }
    else
    {
      if(build_files_simple(dir) != 0) return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PRJ_PUBLIC cmap_prj_public = { main_ };
