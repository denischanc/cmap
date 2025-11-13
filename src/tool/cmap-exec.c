
#include "cmap-exec.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "cmap-config.h"
#include "cmap-compile.h"
#include "cmap-file-util.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static int compile(int argc, char ** argv)
{
  char ** argv_compile = malloc((argc + 1) * sizeof(char *));
  argv_compile[0] = NULL;
  for(int i = 0; i < argc; i++) argv_compile[i + 1] = argv[i];

  int ret = cmap_compile_public.main(argc + 1, argv_compile);
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

static int do_exec(const char * path)
{
  char * bn = cmap_file_util_public.basename_no_ext(path), * tgt = NULL;
  cmap_string_public.append_args(&tgt,
    "%s/%s", cmap_config_public.work_dir(), bn);
  free(bn);

  do_exec_upd_env();
  char * const argv[] = {tgt, NULL};
  execv(tgt, argv);

  fprintf(stderr, "[%s] %s\n", tgt, strerror(errno));
  free(tgt);
  return EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  int ids[] = {CMAP_CONFIG_ID_DEPENDANCE, CMAP_CONFIG_ID_HEADER_DIR,
    CMAP_CONFIG_ID_WORK_DIR, 0};
  cmap_config_public.init_n_check(&argc, &argv, 2,
    CMAP_EXEC_MODULE_NAME " [main cmap file] ([cmap file]...)", ids);

  if(compile(argc, argv) == EXIT_FAILURE) return EXIT_FAILURE;

  return do_exec(argv[1]);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_EXEC_PUBLIC cmap_exec_public = {main_};
