
#include "cmap-compile.h"

#include <stdlib.h>
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
#include "cmap-cli.h"
#include "cmap-fn-name.h"
#include "cmap-usage.h"
#include "cmap-console.h"
#include "cmap-compile-bin.mk.h"
#include "cmap-compile-module.mk.h"

/*******************************************************************************
*******************************************************************************/

char * cmap_compile_exec_path(const char * cmap_path)
{
  char * bn = cmap_file_util_basename_no_ext(cmap_path), * ret = NULL;
  cmap_string_append_args(&ret, "%s/%s", cmap_config_work_dir(), bn);
  free(bn);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void build_add_dep_cflag(const char * cmap_path)
{
  char * cmap_dir = cmap_file_util_dirname(cmap_path);
  DIR * dir = opendir(cmap_dir);
  if(dir == NULL) { free(cmap_dir); return; }

  struct dirent * dirent;
  while((dirent = readdir(dir)) != NULL)
  {
    if(dirent -> d_type == DT_REG)
    {
      const char * extension = cmap_file_util_extension(dirent -> d_name);
      if(!strcmp(extension, "c"))
      {
        char * dep = NULL;
        cmap_string_append_args(&dep, "%s/%s", cmap_dir, dirent -> d_name);
        cmap_config_add_dependance(dep);
        free(dep);
      }
      else if(!strcmp(extension, "h"))
        cmap_config_add_header_dir(cmap_dir);
    }
  }

  closedir(dir);
  free(cmap_dir);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char add_main, only_c, * fn_name;
} BUILD_BUP;

static BUILD_BUP build_bup(const char * cmap_path, char is_main,
  char is_binary)
{
  BUILD_BUP ret;
  ret.add_main = cmap_config_is_add_main();
  ret.only_c = cmap_config_is_only_c();
  const char * fn_name_cur = cmap_config_fn();
  ret.fn_name = (fn_name_cur == NULL) ? NULL : strdup(fn_name_cur);

  cmap_config_set_add_main(is_main && is_binary);
  cmap_config_set_only_c(is_main && is_binary);

  if(is_main)
    cmap_config_set_fn(is_binary ? "cmap_tool_main" : "cmap_module");
  else cmap_fn_name_to_config(cmap_path);

  return ret;
}

static void build_restore(BUILD_BUP bup)
{
  cmap_config_set_add_main(bup.add_main);
  cmap_config_set_only_c(bup.only_c);
  cmap_config_set_fn(bup.fn_name);
  free(bup.fn_name);
}

/*******************************************************************************
*******************************************************************************/

static char build(char * cmap_path, char * root_path, char is_main,
  char is_binary)
{
  BUILD_BUP bup = build_bup(cmap_path, is_main, is_binary);

  char * argv[] = {CMAP_BUILD_MODULE_NAME, cmap_path, root_path};
  char ret = (cmap_build_main(3, argv) == EXIT_SUCCESS);

  build_restore(bup);

  return ret;
}

static char build_all(int argc, char ** argv, const char * work_dir,
  char is_binary)
{
  char ret = (1 == 1);

  for(int j = 0; ret && (j < argc); j++)
  {
    char * cmap_path = argv[j], * root_path = NULL, * c_path = NULL,
      * bn = cmap_file_util_basename_no_ext(cmap_path);
    cmap_string_append_args(&root_path, "%s/%s", work_dir, bn);
    cmap_string_append_args(&c_path, "%s.c", root_path);

    cmap_config_add_dependance(c_path);
    build_add_dep_cflag(cmap_path);

    ret = build(cmap_path, root_path, j == 0, is_binary);

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
  cmap_string_append_args(deps, " %s", tmp != NULL ? tmp : dep);
  free(tmp);
}

static void cflags_apply(const char * cflag, void * cflags)
{
  char * tmp = realpath(cflag, NULL);
  cmap_string_append_args(cflags, " -I%s", tmp != NULL ? tmp : cflag);
  free(tmp);
}

static void ldflags_dir_apply(const char * dir, void * ldflags)
{
  char * tmp = realpath(dir, NULL);
  cmap_string_append_args(ldflags, " -L%s", tmp != NULL ? tmp : dir);
  free(tmp);
}

static void ldflags_lib_apply(const char * lib, void * ldflags)
{
  cmap_string_append_args(ldflags, " -l%s", lib);
}

static char create_mk(const char * tgt, const char * work_dir, char is_binary)
{
  char * deps = NULL, * cflags = strdup(""), * ldflags = strdup("");
  cmap_strings_apply(cmap_config_dependance(), deps_apply, &deps);
  cmap_strings_apply(cmap_config_header_dir(), cflags_apply, &cflags);
  cmap_strings_apply(cmap_config_lib_dir(), ldflags_dir_apply, &ldflags);
  cmap_strings_apply(cmap_config_lib(), ldflags_lib_apply, &ldflags);

  char * path_mk = NULL;
  cmap_string_append_args(&path_mk, "%s/%s.mk", work_dir, tgt);

  const char * txt = is_binary ? CMAP_COMPILE_BIN_MK : CMAP_COMPILE_MODULE_MK;
  char ret = cmap_file_util_to_file(path_mk, txt, cflags, ldflags, tgt, deps);

  free(deps);
  free(cflags);
  free(ldflags);
  free(path_mk);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char system_make(const char * tgt, const char * work_dir)
{
  char * command = NULL;
  cmap_string_append_args(&command, "make%s -C %s -f %s.mk",
    cmap_config_is_quiet() ? " -s" : "", work_dir, tgt);

  int ret = system(command);
  if(ret < 0)
  {
    cmap_console_error("[%s] %s\n", command, strerror(errno));
    ret = EXIT_FAILURE;
  }

  free(command);

  return (ret == EXIT_SUCCESS);
}

/*******************************************************************************
*******************************************************************************/

#define DESC " [main cmap file] ([cmap file]...) %s"

int cmap_compile_main(int argc, char * argv[])
{
  char is_binary = !strcmp(argv[0], CMAP_COMPILE_MODULE_NAME);

  if((argc < 2) || cmap_config_is_help())
  {
    int ids[] = {CMAP_CLI_ID_DEPENDANCE, CMAP_CLI_ID_HEADER_DIR,
      CMAP_CLI_ID_LIB_DIR, CMAP_CLI_ID_LIB, CMAP_CLI_ID_WORK_DIR, 0};
    const char * desc = is_binary ? CMAP_COMPILE_MODULE_NAME DESC :
      CMAP_COMPILE_MODULE_MODULE_NAME DESC;
    return cmap_usage(desc, ids);
  }

  const char * work_dir = cmap_config_work_dir();
  if((access(work_dir, F_OK) < 0) && (mkdir(work_dir, 0755) < 0))
  {
    cmap_console_error("[%s] %s\n", work_dir, strerror(errno));
    return EXIT_FAILURE;
  }

  if(!build_all(argc - 1, argv + 1, work_dir, is_binary)) return EXIT_FAILURE;

  char ret = (1 == 1);

  char * tgt = cmap_file_util_basename_no_ext(argv[1]);
  if(!is_binary) cmap_string_append(&tgt, ".so");
  if(!create_mk(tgt, work_dir, is_binary)) ret = (1 == 0);
  if(ret && !system_make(tgt, work_dir)) ret = (1 == 0);
  free(tgt);

  return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
