#ifndef __CMAP_CONFIG_DEFINE_H__
#define __CMAP_CONFIG_DEFINE_H__

#include "cmap-strings.h"

#define CMAP_CONFIG_LOOP(macro_bool, macro_string, macro_strings) \
  macro_bool(relative_inc, RELATIVE_INC) \
  macro_string(fn, FN, NULL) \
  macro_strings(header_dir, HEADER_DIR) \
  macro_bool(only_c, ONLY_C) \
  macro_bool(add_main, ADD_MAIN) \
  macro_bool(quiet, QUIET) \
  macro_string(include, INCLUDE, NULL) \
  macro_bool(multiple, MULTIPLE) \
  macro_string(work_dir, WORK_DIR, ".cmap") \
  macro_strings(dependance, DEPENDANCE) \
  macro_bool(help, HELP) \
  macro_bool(version, VERSION) \
  macro_strings(lib_dir, LIB_DIR) \
  macro_strings(lib, LIB)

#define CMAP_CONFIG_BOOL_DECL(name, ENV_VAR) \
  void cmap_config_set_##name(char v); \
  char cmap_config_is_##name();

#define CMAP_CONFIG_STRING_DECL(name, ENV_VAR, dft) \
  void cmap_config_set_##name(const char * v); \
  const char * cmap_config_##name();

#define CMAP_CONFIG_STRINGS_DECL(name, ENV_VAR) \
  void cmap_config_add_##name(const char * v); \
  CMAP_STRINGS * cmap_config_##name();

#endif
