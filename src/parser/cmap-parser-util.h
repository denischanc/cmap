#ifndef __CMAP_PARSER_UTIL_H__
#define __CMAP_PARSER_UTIL_H__

#include <cmap/core.h>
#include <cmap/parser-util.h>
#include "cmap-scanner.h"

struct SCANNER_NODE_s
{
  yyscan_t scanner;
  SCANNER_NODE * next;
};

typedef struct
{
  void (*$$)(CMAP_MAP * definitions, const char * impl, SCANNER_NODE ** chain);

  CMAP_MAP * (*name)(CMAP_MAP * definitions, const char * name);
  CMAP_MAP * (*path)(CMAP_MAP * map, const char * name);

  void (*set_local)(CMAP_MAP * definitions, const char * name, CMAP_MAP * map);
  void (*set_global)(const char * name, CMAP_MAP * map);
  void (*set_path)(CMAP_MAP * src, const char * name, CMAP_MAP * map);

  CMAP_LIST * (*args_empty)();
  CMAP_LIST * (*args_push)(CMAP_LIST * list, CMAP_MAP * map);
  CMAP_LIST * (*args)(CMAP_MAP * map);

  CMAP_LIST * (*args_map_push)(CMAP_LIST * list, const char * name,
    CMAP_MAP * map);
  CMAP_LIST * (*args_map)(const char * name, CMAP_MAP * map);

  CMAP_MAP * (*map_args)(CMAP_LIST * args, const char * aisle);

  CMAP_MAP * (*list_args)(CMAP_LIST * args, const char * aisle);

  CMAP_STRING * (*string_scanner)();
  void (*scanner_append)(CMAP_STRING * string, const char * txt);
  CMAP_MAP * (*string)(CMAP_STRING * string, const char * aisle);

  CMAP_MAP * (*process)(CMAP_MAP * definitions, CMAP_MAP * map,
    const char * fn_name, CMAP_LIST * args);
} CMAP_PARSER_UTIL_PUBLIC;

extern const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public;

#endif
