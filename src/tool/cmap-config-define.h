#ifndef __CMAP_CONFIG_DEFINE_H__
#define __CMAP_CONFIG_DEFINE_H__

#include "cmap-strings.h"

#define CMAP_CONFIG_LOOP(macro_bool, macro_string, macro_strings) \
  macro_bool(relative_inc, CMAP_RELATIVE_INC) \
  macro_string(fn, CMAP_FN, NULL) \
  macro_strings(header_dir, CMAP_HEADER_DIR) \
  macro_bool(only_c, CMAP_ONLY_C) \
  macro_bool(add_main, CMAP_ADD_MAIN) \
  macro_bool(quiet, CMAP_QUIET) \
  macro_string(include, CMAP_INCLUDE, NULL) \
  macro_bool(multiple, CMAP_MULTIPLE) \
  macro_string(work_dir, CMAP_WORK_DIR, ".cmap") \
  macro_strings(dependance, CMAP_DEPENDANCE) \
  macro_bool(help, CMAP_HELP) \
  macro_bool(version, CMAP_VERSION) \
  macro_strings(lib_dir, CMAP_LIB_DIR) \
  macro_strings(lib, CMAP_LIB)

#define CMAP_CONFIG_BOOL_DECL(name, env_var) \
  void (*set_##name)(char v); \
  char (*is_##name)();

#define CMAP_CONFIG_STRING_DECL(name, env_var, dft) \
  void (*set_##name)(const char * v); \
  const char * (*name)();

#define CMAP_CONFIG_STRINGS_DECL(name, env_var) \
  void (*add_##name)(const char * v); \
  CMAP_STRINGS * (*name)();

#endif
