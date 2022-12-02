#ifndef __CMAP_PARSER_UTIL_H__
#define __CMAP_PARSER_UTIL_H__

#include <cmap/core.h>
#include "cmap-proc-ctx.h"
#include "cmap-parser-util-define.h"

typedef struct
{
  CMAP_MAP * (*proc_impl)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * definitions,
    const char * impl);

  CMAP_MAP * (*name)(CMAP_PROC_CTX * proc_ctx, const char * name);
  CMAP_MAP * (*path)(CMAP_MAP * map, const char * name);

  void (*set_local)(CMAP_PROC_CTX * proc_ctx, const char * name,
    CMAP_MAP * map);
  void (*set_global)(CMAP_PROC_CTX * proc_ctx, const char * name,
    CMAP_MAP * map);
  void (*set_path)(CMAP_MAP * src, const char * name, CMAP_MAP * map);

  CMAP_LIST * (*args_empty)(CMAP_PROC_CTX * proc_ctx);
  CMAP_LIST * (*args_push)(CMAP_LIST * list, CMAP_MAP * map);
  CMAP_LIST * (*args)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map);

  CMAP_LIST * (*args_name_push)(CMAP_PROC_CTX * proc_ctx, CMAP_LIST * list,
    const char * name);
  CMAP_LIST * (*args_name)(CMAP_PROC_CTX * proc_ctx, const char * name);

  CMAP_LIST * (*args_map_push)(CMAP_PROC_CTX * proc_ctx, CMAP_LIST * list,
    const char * name, CMAP_MAP * map);
  CMAP_LIST * (*args_map)(CMAP_PROC_CTX * proc_ctx, const char * name,
    CMAP_MAP * map);

  CMAP_MAP * (*map_args)(CMAP_LIST * args, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);

  CMAP_MAP * (*list_args)(CMAP_LIST * args, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);

  CMAP_STRING * (*string_scanner)(CMAP_PROC_CTX * proc_ctx);
  void (*scanner_append)(CMAP_STRING * string, const char * txt);
  CMAP_MAP * (*string)(CMAP_STRING * string, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);

  CMAP_MAP * (*int_)(int64_t i, CMAP_PROC_CTX * proc_ctx, const char * aisle);

  CMAP_MAP * (*process)(CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, const char * fn_name, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_MAP * map, const char * fn_name, CMAP_LIST * args,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);

  CMAP_MAP * (*function)(CMAP_PROC_CTX * proc_ctx, const char * aisle,
    CMAP_LIST * args, CMAP_STRING * impl);

  CMAP_PARSER_UTIL_CMP_LOOP(CMAP_PARSER_UTIL_CMP_DECL)
} CMAP_PARSER_UTIL_PUBLIC;

extern const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public;

#endif
