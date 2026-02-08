
#include "cmap-exec.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "cmap-config.h"
#include "cmap-compile.h"
#include "cmap-file-util.h"
#include "cmap-string.h"
#include "cmap-usage.h"
#include "cmap-console.h"

/*******************************************************************************
*******************************************************************************/

static int compile(int argc, char ** argv)
{
  char ** argv_compile = malloc((argc + 1) * sizeof(char *));
  argv_compile[0] = CMAP_COMPILE_MODULE_NAME;
  for(int i = 0; i < argc; i++) argv_compile[i + 1] = argv[i];

  char quiet_bup = cmap_config_public.is_quiet();
  cmap_config_public.set_quiet(1 == 1);

  int ret = cmap_compile_public.main(argc + 1, argv_compile);

  cmap_config_public.set_quiet(quiet_bup);

  free(argv_compile);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void do_exec_upd_env()
{
  char * val = NULL;

  const char * cur = getenv("LD_LIBRARY_PATH");
  if(cur != NULL) cmap_string_public.append_args(&val, "%s:", cur);

  cmap_string_public.append(&val, CMAP_LIBDIR);

  setenv("LD_LIBRARY_PATH", val, 1);
  free(val);
}

static int do_exec(const char * cmap_path, int argc, char ** argv)
{
  char * exec_path = cmap_compile_public.exec_path(cmap_path);

  do_exec_upd_env();
  char ** argv_exec = malloc((argc + 2) * sizeof(char *));
  argv_exec[0] = exec_path;
  for(int i = 0; i < argc; i++) argv_exec[i + 1] = argv[i];
  argv_exec[argc + 1] = NULL;
  execv(exec_path, argv_exec);

  cmap_console_public.error("[%s] %s\n", exec_path, strerror(errno));
  free(exec_path);
  free(argv_exec);
  return EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if((argc < 2) || cmap_config_public.is_help())
  {
    int ids[] = {CMAP_CONFIG_ID_DEPENDANCE, CMAP_CONFIG_ID_HEADER_DIR,
      CMAP_CONFIG_ID_WORK_DIR, 0};
    return cmap_usage_public.usage(CMAP_EXEC_MODULE_NAME
      " [main cmap file] ([cmap file]...) %s (-- [exec args]...)", ids);
  }

  int exec_args_off = argc, argc_compile = argc - 1;
  for(int i = 2; (i < argc) && (exec_args_off == argc); i++)
  {
    if(!strcmp(argv[i], "--"))
    {
      exec_args_off = i + 1;
      argc_compile = i - 1;
    }
  }

  if(compile(argc_compile, argv + 1) == EXIT_FAILURE) return EXIT_FAILURE;

  return do_exec(argv[1], argc - exec_args_off, argv + exec_args_off);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_EXEC_PUBLIC cmap_exec_public = {main_};
