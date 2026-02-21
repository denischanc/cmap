#ifndef __CMAP_PARSER_UTIL_H__
#define __CMAP_PARSER_UTIL_H__

#include "cmap-parser-util-define.h"

void cmap_parser_util_append_instruction_args(const char * txt, ...);
void cmap_parser_util_append_variable_args(const char * txt, ...);
void cmap_parser_util_prepend_instruction_args(const char * txt, ...);
void cmap_parser_util_append_instruction(const char * txt);
void cmap_parser_util_append_lf();

void cmap_parser_util_append_instruction_args_args(char * args,
  const char * txt, ...);

const char * cmap_parser_util_add_definitions();
const char * cmap_parser_util_add_global_env();

void cmap_parser_util_prepend_map_var(const char * map_name);

char * cmap_parser_util_next_name(const char * what);

#endif
