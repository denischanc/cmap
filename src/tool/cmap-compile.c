
#include "cmap-compile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "cmap-build.h"
#include "cmap-file-util.h"
#include "cmap-string.h"
#include "cmap-config.h"
#include "cmap-compile-mk.h"
#include "cmap-fn-name.h"

/*******************************************************************************
*******************************************************************************/

static char * exec_path(const char * cmap_path)
{
  char * bn = cmap_file_util_public.basename_no_ext(cmap_path), * ret = NULL;
  cmap_string_public.append_args(&ret,
    "%s/%s", cmap_config_public.work_dir(), bn);
  free(bn);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void build_add_dep_cflag(const char * cmap_path)
{
  char * cmap_dir = cmap_file_util_public.dirname(cmap_path);
  DIR * dir = opendir(cmap_dir);
  if(dir == NULL) { free(cmap_dir); return; }

  struct dirent * dirent;
  while((dirent = readdir(dir)) != NULL)
  {
    if(dirent -> d_type == DT_REG)
    {
      const char * extension =
        cmap_file_util_public.extension(dirent -> d_name);
      if(!strcmp(extension, "c"))
      {
        char * dep = NULL;
        cmap_string_public.append_args(&dep,
          "%s/%s", cmap_dir, dirent -> d_name);
        cmap_config_public.add_dependance(dep);
        free(dep);
      }
      else if(!strcmp(extension, "h"))
        cmap_config_public.add_header_dir(cmap_dir);
    }
  }

  closedir(dir);
  free(cmap_dir);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char add_main, only_c, quiet, * fn_name;
} BUILD_BUP;

static BUILD_BUP build_bup(const char * cmap_path, char add_main)
{
  BUILD_BUP ret;
  ret.add_main = cmap_config_public.is_add_main();
  ret.only_c = cmap_config_public.is_only_c();
  ret.quiet = cmap_config_public.is_quiet();
  const char * fn_name_cur = cmap_config_public.fn();
  ret.fn_name = (fn_name_cur == NULL) ? NULL : strdup(fn_name_cur);

  cmap_config_public.set_add_main(add_main);
  cmap_config_public.set_only_c(add_main);
  cmap_config_public.set_quiet(1 == 1);

  if(add_main) cmap_config_public.set_fn("cmap_tool_main");
  else cmap_fn_name_public.to_config(cmap_path);

  return ret;
}

static void build_restore(BUILD_BUP bup)
{
  cmap_config_public.set_add_main(bup.add_main);
  cmap_config_public.set_only_c(bup.only_c);
  cmap_config_public.set_quiet(bup.quiet);
  cmap_config_public.set_fn(bup.fn_name);
  free(bup.fn_name);
}

/*******************************************************************************
*******************************************************************************/

static char build(char * cmap_path, char * root_path, char add_main)
{
  BUILD_BUP bup = build_bup(cmap_path, add_main);

  char * argv[] = {CMAP_BUILD_MODULE_NAME, cmap_path, root_path};
  char ret = (cmap_build_public.main(3, argv) == EXIT_SUCCESS);

  build_restore(bup);

  return ret;
}

static char build_all(int argc, char ** argv, const char * work_dir)
{
  char ret = (1 == 1);

  for(int j = 0; ret && (j < argc); j++)
  {
    char * cmap_path = argv[j], * root_path = NULL, * c_path = NULL,
      * bn = cmap_file_util_public.basename_no_ext(cmap_path);
    cmap_string_public.append_args(&root_path, "%s/%s", work_dir, bn);
    cmap_string_public.append_args(&c_path, "%s.c", root_path);

    cmap_config_public.add_dependance(c_path);
    build_add_dep_cflag(cmap_path);

    ret = build(cmap_path, root_path, j == 0);

    free(root_path);
    free(c_path);
    free(bn);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void deps_apply(const char * dep, void * deps)
{
  char * tmp = realpath(dep, NULL);
  cmap_string_public.append_args(deps, " %s", tmp);
  free(tmp);
}

static void cflags_apply(const char * cflag, void * cflags)
{
  char * tmp = realpath(cflag, NULL);
  cmap_string_public.append_args(cflags, " -I%s", tmp);
  free(tmp);
}

static char create_mk(const char * tgt, const char * work_dir)
{
  char * deps = NULL, * cflags = strdup("");
  cmap_strings_public.apply(cmap_config_public.dependance(), deps_apply,
    &deps);
  cmap_strings_public.apply(cmap_config_public.header_dir(), cflags_apply,
    &cflags);

  char * path_mk = NULL;
  cmap_string_public.append_args(&path_mk, "%s/%s.mk", work_dir, tgt);

  char ret = cmap_file_util_public.to_file(
    path_mk, CMAP_COMPILE_MK, cflags, tgt, deps);

  free(deps);
  free(cflags);
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

static int main_(int argc, char * argv[])
{
  if(argc < 2)
  {
    int ids[] = {CMAP_CONFIG_ID_DEPENDANCE, CMAP_CONFIG_ID_HEADER_DIR,
      CMAP_CONFIG_ID_WORK_DIR, 0};
    return cmap_config_public.usage(
      CMAP_COMPILE_MODULE_NAME " [main cmap file] ([cmap file]...) %s", ids);
  }

  const char * work_dir = cmap_config_public.work_dir();
  if((access(work_dir, F_OK) < 0) && (mkdir(work_dir, 0755) < 0))
  {
    fprintf(stderr, "[%s] %s\n", work_dir, strerror(errno));
    return EXIT_FAILURE;
  }

  if(!build_all(argc - 1, argv + 1, work_dir)) return EXIT_FAILURE;

  char ret = (1 == 1);

  char * tgt = cmap_file_util_public.basename_no_ext(argv[1]);
  if(!create_mk(tgt, work_dir)) ret = (1 == 0);
  if(ret && !system_make(tgt, work_dir)) ret = (1 == 0);
  free(tgt);

  return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_COMPILE_PUBLIC cmap_compile_public = {exec_path, main_};
