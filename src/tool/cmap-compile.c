
#include "cmap-compile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "cmap-build.h"
#include "cmap-file-util.h"
#include "cmap-string.h"
#include "cmap-config.h"
#include "cmap-compile-mk.h"

/*******************************************************************************
*******************************************************************************/

static char build(const char * path_cmap, const char * path_root, char add_main)
{
  cmap_config_public.set_add_main(add_main);
  cmap_config_public.set_only_c(add_main);
  cmap_config_public.set_quiet(1 == 1);

  const char * argv[] = {NULL, CMAP_BUILD_MODULE_NAME, path_cmap, path_root};
  int ret = cmap_build_public.main(4, (char **)argv);

  return (ret == EXIT_SUCCESS);
}

static char build_all(int argc, char ** argv, char ** tgt, char ** deps,
  const char * work_dir)
{
  char ret = (1 == 1);

  for(int j = 0; ret && (j < argc); j++)
  {
    char * bn = cmap_file_util_public.basename_no_ext(argv[j]),
      * path_root = NULL;
    cmap_string_public.append_args(&path_root, "%s/%s", work_dir, bn);

    if(j == 0) *tgt = strdup(bn);
    cmap_string_public.append_args(deps, " %s.c", bn);
    free(bn);

    ret = build(argv[j], path_root, j == 0);

    free(path_root);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char create_mk(const char * tgt, const char * deps, const char * cflags,
  const char * work_dir)
{
  char * path_mk = NULL;
  cmap_string_public.append_args(&path_mk, "%s/%s.mk", work_dir, tgt);

  char ret = cmap_file_util_public.to_file(
    path_mk, CMAP_COMPILE_MK, cflags, tgt, deps);

  free(path_mk);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char system_make(const char * tgt, const char * work_dir)
{
  char * command = NULL;
  cmap_string_public.append_args(
    &command, "make -C %s -f %s.mk", work_dir, tgt);

  int ret = system(command);
  if(ret < 0)
  {
    fprintf(stderr, "[%s] %s\n", command, strerror(errno));
    ret = EXIT_FAILURE;
  }

  free(command);

  return (ret == EXIT_SUCCESS);
}

/*******************************************************************************
*******************************************************************************/

static void deps_apply(const char * string, void * data)
{
  char * tmp = realpath(string, NULL);
  cmap_string_public.append_args(data, " %s", tmp);
  free(tmp);
}

static void cflags_apply(const char * string, void * data)
{
  char * tmp = realpath(string, NULL);
  cmap_string_public.append_args(data, " -I%s", tmp);
  free(tmp);
}

static int main_(int argc, char * argv[])
{
  if(argc < 2)
  {
    int ids[] = {CMAP_CONFIG_ID_DEPENDANCE, CMAP_CONFIG_ID_HEADER_DIR,
      CMAP_CONFIG_ID_WORK_DIR, 0};
    return cmap_config_public.usage(
      CMAP_COMPILE_MODULE_NAME " [main cmap file] ([cmap file]...)", ids);
  }

  char ret = (1 == 1), * tgt = NULL, * deps = NULL, * cflags = strdup("");

  cmap_strings_public.apply(cmap_config_public.dependance(), deps_apply,
    &deps);
  cmap_strings_public.apply(cmap_config_public.header_dir(), cflags_apply,
    &cflags);

  const char * work_dir = cmap_config_public.work_dir();
  if((access(work_dir, F_OK) < 0) && (mkdir(work_dir, 0755) < 0))
  {
    fprintf(stderr, "[%s] %s\n", work_dir, strerror(errno));
    ret = (1 == 0);
  }

  if(ret && !build_all(argc - 1, argv + 1, &tgt, &deps, work_dir))
    ret = (1 == 0);

  if(ret && !create_mk(tgt, deps, cflags, work_dir)) ret = (1 == 0);
  if(ret && !system_make(tgt, work_dir)) ret = (1 == 0);

  free(tgt);
  free(deps);
  free(cflags);

  return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_COMPILE_PUBLIC cmap_compile_public = {main_};
