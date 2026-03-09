
#include "cmap-prj.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
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
#include "cmap-prj-module-Makefile.h"
#include "cmap-prj-module-watch-mem.cmap.h"
#include "cmap-prj-module-watch-mem-fn.c.h"
#include "cmap-prj-module-watch-mem-fn.h.h"
#include "cmap-config.h"
#include "cmap-cli.h"
#include "cmap-usage.h"
#include "cmap-console.h"

/*******************************************************************************
*******************************************************************************/

static char to_file(const char * dir, const char * file_name, const char * txt)
{
  char * path = NULL;
  cmap_string_append_args(&path, "%s/%s", dir, file_name);
  char ret = cmap_file_util_to_file(path, txt);
  free(path);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char build_files(const char * dir, ...)
{
  char ret = (1 == 1);

  va_list fn_txts;
  va_start(fn_txts, dir);

  const char * fn, * txt;
  while(((fn = va_arg(fn_txts, const char *)) != NULL) && ret)
  {
    txt = va_arg(fn_txts, const char *);
    ret = to_file(dir, fn, txt);
  }

  va_end(fn_txts);

  return ret;
}

static char build_files_simple(const char * dir)
{
  return build_files(dir,
    "hello-world.cmap", CMAP_PRJ_SIMPLE_HELLO_WORLD_CMAP,
    "Makefile", CMAP_PRJ_SIMPLE_MAKEFILE,
    NULL);
}

static char build_files_multiple(const char * dir)
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

static char build_files_module(const char * dir)
{
  return build_files(dir,
    "Makefile", CMAP_PRJ_MODULE_MAKEFILE,
    "watch-mem.cmap", CMAP_PRJ_MODULE_WATCH_MEM_CMAP,
    "watch-mem-fn.c", CMAP_PRJ_MODULE_WATCH_MEM_FN_C,
    "watch-mem-fn.h", CMAP_PRJ_MODULE_WATCH_MEM_FN_H,
    NULL);
}

/*******************************************************************************
*******************************************************************************/

#define PRJ_LOOP(macro) \
  macro(SIMPLE, simple) \
  macro(MULTIPLE, multiple) \
  macro(MODULE, module)

#define PRJ_BUILD(NAME, name) \
  if(!strcmp(argv[0], CMAP_PRJ_##NAME##_MODULE_NAME)) \
  { \
    if(!build_files_##name(dir)) return EXIT_FAILURE; \
  }

int cmap_prj_main(int argc, char * argv[])
{
  if((argc < 2) || cmap_config_is_help())
  {
    char desc[50];
    snprintf(desc, sizeof(desc), "%s [project directory]", argv[0]);
    return cmap_usage(desc, NULL);
  }

  char * dir = argv[1];
  if(mkdir(dir, 0755) < 0)
  {
    cmap_console_error("[%s] %s\n", dir, strerror(errno));
    return EXIT_FAILURE;
  }

  PRJ_LOOP(PRJ_BUILD)

  return EXIT_SUCCESS;
}
