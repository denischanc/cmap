#ifndef __CMAP_PARSER_PART_DEFINE_H__
#define __CMAP_PARSER_PART_DEFINE_H__

#include "cmap-part.h"

#define CMAP_PARSER_PART_LOOP(macro) \
  CMAP_PART_LOOP(macro) \
  macro(instructions)

#define CMAP_PARSER_PART_APPEND_DECL(part) \
  void (*append_args_##part)(const char * txt, ...); \
  void (*append_##part)(const char * txt);

#endif
