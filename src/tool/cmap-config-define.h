#ifndef __CMAP_CONFIG_DEFINE_H__
#define __CMAP_CONFIG_DEFINE_H__

#include "cmap-strings.h"

#define CMAP_CONFIG_ID_RELATIVE_INC 1
#define CMAP_CONFIG_ID_FN 2
#define CMAP_CONFIG_ID_HEADER_DIR 3
#define CMAP_CONFIG_ID_ONLY_C 4
#define CMAP_CONFIG_ID_ADD_MAIN 5
#define CMAP_CONFIG_ID_QUIET 6
#define CMAP_CONFIG_ID_INCLUDE 7
#define CMAP_CONFIG_ID_MULTIPLE 8
#define CMAP_CONFIG_ID_WORK_DIR 9
#define CMAP_CONFIG_ID_DEPENDANCE 10

#define CMAP_CONFIG_LOOP(macro_bool, macro_string, macro_strings) \
  macro_bool(CMAP_CONFIG_ID_RELATIVE_INC, relative_inc, relative-inc, 'i', \
    CMAP_RELATIVE_INC, "Relative include") \
  macro_string(CMAP_CONFIG_ID_FN, fn, fn, 'f', CMAP_FN, "Function name", \
    NULL) \
  macro_strings(CMAP_CONFIG_ID_HEADER_DIR, header_dir, header-dir, 'I', \
    CMAP_HEADER_DIR, "Add searched header dir") \
  macro_bool(CMAP_CONFIG_ID_ONLY_C, only_c, only-c, 'c', CMAP_ONLY_C, \
    "Only c generation") \
  macro_bool(CMAP_CONFIG_ID_ADD_MAIN, add_main, add-main, 'm', CMAP_ADD_MAIN, \
    "Add main") \
  macro_bool(CMAP_CONFIG_ID_QUIET, quiet, quiet, 'q', CMAP_QUIET, "Be quiet") \
  macro_string(CMAP_CONFIG_ID_INCLUDE, include, include, 'l', CMAP_INCLUDE, \
    "Include to add", NULL) \
  macro_bool(CMAP_CONFIG_ID_MULTIPLE, multiple, multiple, 'p', CMAP_MULTIPLE, \
    "Multiple") \
  macro_string(CMAP_CONFIG_ID_WORK_DIR, work_dir, work-dir, 'W', \
    CMAP_WORK_DIR, "Working directory", ".cmap") \
  macro_strings(CMAP_CONFIG_ID_DEPENDANCE, dependance, dependance, 'd', \
    CMAP_DEPENDANCE, "Add dependance")

#define CMAP_CONFIG_BOOL_DECL(ID, name, long_opt, opt, env_var, desc) \
  void (*set_##name)(char v); \
  char (*is_##name)();

#define CMAP_CONFIG_STRING_DECL(ID, name, long_opt, opt, env_var, desc, dft) \
  void (*set_##name)(const char * v); \
  const char * (*name)();

#define CMAP_CONFIG_STRINGS_DECL(ID, name, long_opt, opt, env_var, desc) \
  void (*add_##name)(const char * v); \
  CMAP_STRINGS * (*name)();

#endif
