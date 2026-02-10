#ifndef __CMAP_CLI_DEFINE_H__
#define __CMAP_CLI_DEFINE_H__

#include "cmap-strings.h"

#define CMAP_CLI_ID_RELATIVE_INC 1
#define CMAP_CLI_ID_FN 2
#define CMAP_CLI_ID_HEADER_DIR 3
#define CMAP_CLI_ID_ONLY_C 4
#define CMAP_CLI_ID_ADD_MAIN 5
#define CMAP_CLI_ID_QUIET 6
#define CMAP_CLI_ID_INCLUDE 7
#define CMAP_CLI_ID_MULTIPLE 8
#define CMAP_CLI_ID_WORK_DIR 9
#define CMAP_CLI_ID_DEPENDANCE 10
#define CMAP_CLI_ID_HELP 11
#define CMAP_CLI_ID_VERSION 12
#define CMAP_CLI_ID_LIB_DIR 13
#define CMAP_CLI_ID_LIB 14

#define CMAP_CLI_LOOP(macro_bool, macro_string, macro_strings) \
  macro_bool(RELATIVE_INC, relative_inc, relative-inc, 'r', \
    "Relative include") \
  macro_string(FN, fn, fn, 'f', "Function name") \
  macro_strings(HEADER_DIR, header_dir, header-dir, 'I', \
    "Add searched header dir") \
  macro_bool(ONLY_C, only_c, only-c, 'c', "Only c generation") \
  macro_bool(ADD_MAIN, add_main, add-main, 'm', "Add main") \
  macro_bool(QUIET, quiet, quiet, 'q', "Be quiet") \
  macro_string(INCLUDE, include, include, 'n', "Include to add") \
  macro_bool(MULTIPLE, multiple, multiple, 'p', "Multiple") \
  macro_string(WORK_DIR, work_dir, work-dir, 'W', "Working directory") \
  macro_strings(DEPENDANCE, dependance, dependance, 'd', "Add dependance") \
  macro_bool(HELP, help, help, 'h', "Display usage") \
  macro_bool(VERSION, version, version, 'V', "Display version") \
  macro_strings(LIB_DIR, lib_dir, lib-dir, 'L', "Add searched lib dir") \
  macro_strings(LIB, lib, lib, 'l', "Add lib")

#define CMAP_CLI_BOOL_DECL(ID, name, long_opt, opt, desc) \
  char (*is_##name)();

#define CMAP_CLI_STRING_DECL(ID, name, long_opt, opt, desc) \
  const char * (*name)();

#define CMAP_CLI_STRINGS_DECL(ID, name, long_opt, opt, desc) \
  CMAP_STRINGS * (*name)();

#endif
