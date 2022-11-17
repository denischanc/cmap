
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-fn.h"
#include "cmap-parser.h"

/*******************************************************************************
*******************************************************************************/

static SCANNER_NODE ** push_scanner(SCANNER_NODE ** chain, yyscan_t scanner)
{
  if(chain == NULL)
  {
    chain = CMAP_KERNEL_ALLOC(SCANNER_NODE *);
    *chain = NULL;
  }

  CMAP_KERNEL_ALLOC_PTR(node, SCANNER_NODE);
  node -> scanner = scanner;
  node -> next = *chain;
  *chain = node;

  return chain;
}

static char is_scanner(SCANNER_NODE ** chain, yyscan_t scanner)
{
  return ((*chain) -> scanner == scanner);
}

static void pop_scanner(SCANNER_NODE ** chain)
{
  SCANNER_NODE * node = *chain;
  *chain = (*chain) -> next;
  CMAP_KERNEL_FREE(node);

  if(*chain == NULL) CMAP_KERNEL_FREE(chain);
}

/*******************************************************************************
*******************************************************************************/

static void $$(CMAP_MAP * definitions, char * impl, SCANNER_NODE ** chain)
{
  yyscan_t scanner;
  cmap_parser_lex_init(&scanner);
  cmap_parser_set_in(fmemopen(impl, strlen(impl), "r"), scanner);

  chain = push_scanner(chain, scanner);
  cmap_parser_parse(scanner, definitions, chain);
  pop_scanner(chain);

  fclose(cmap_parser_get_in(scanner));
  cmap_parser_lex_destroy(scanner);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * name(CMAP_MAP * definitions, const char * name)
{
  CMAP_MAP * map = NULL;

  if((definitions != NULL) && CMAP_CALL_ARGS(definitions, is_key, name))
    map = definitions;
  else map = cmap_kernel_public.global_env();

  CMAP_MAP * ret = CMAP_GET(map, name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * path(CMAP_MAP * map, const char * name)
{
  return CMAP_GET(map, name);
}

/*******************************************************************************
*******************************************************************************/

static void set_local(CMAP_MAP * definitions,
  const char * name, CMAP_MAP * map)
{
  CMAP_SET(definitions, name, map);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(const char * name, CMAP_MAP * map)
{
  CMAP_SET(cmap_kernel_public.global_env(), name, map);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(CMAP_MAP * src, const char * name, CMAP_MAP * map)
{
  CMAP_SET(src, name, map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * args_push(CMAP_LIST * list, CMAP_MAP * map)
{
  return CMAP_LIST_PUSH(list, map);
}

static CMAP_LIST * args(CMAP_MAP * map)
{
  return args_push(CMAP_CALL(cmap_kernel_public.pool_list(), take), map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * args_map_push(CMAP_LIST * list, const char * name,
  CMAP_MAP * map)
{
  CMAP_STRING * name_s = CMAP_CALL(cmap_kernel_public.pool_string(), take);
  CMAP_CALL_ARGS(name_s, append, name);
  CMAP_LIST_PUSH(list, name_s);
  return CMAP_LIST_PUSH(list, map);
}

static CMAP_LIST * args_map(const char * name, CMAP_MAP * map)
{
  return args_map_push(CMAP_CALL(cmap_kernel_public.pool_list(), take),
    name, map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * map(const char * aisle)
{
  return CMAP_MAP(aisle);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * map_args(CMAP_LIST * args, const char * aisle)
{
  CMAP_MAP * map = CMAP_MAP(aisle);

  CMAP_STRING * key;
  while((key = (CMAP_STRING *)CMAP_LIST_SHIFT(args)) != NULL)
  {
    CMAP_MAP * val = CMAP_LIST_SHIFT(args);
    CMAP_SET(map, CMAP_CALL(key, val), val);
    CMAP_CALL_ARGS(cmap_kernel_public.pool_string(), release, key);
  }
  CMAP_CALL_ARGS(cmap_kernel_public.pool_list(), release, args);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * list(const char * aisle)
{
  return (CMAP_MAP *)CMAP_LIST(0, aisle);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * list_args(CMAP_LIST * args, const char * aisle)
{
  CMAP_LIST * list = CMAP_LIST(0, aisle);

  int size = CMAP_CALL(args, size);
  for(int i = 0; i < size; i++) CMAP_LIST_PUSH(list, CMAP_LIST_SHIFT(args));
  CMAP_CALL_ARGS(cmap_kernel_public.pool_list(), release, args);

  return (CMAP_MAP *)list;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRING * string_scanner()
{
  CMAP_STRING * ret = CMAP_CALL(cmap_kernel_public.pool_string(), take);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void scanner_append(CMAP_STRING * string, const char * txt)
{
  CMAP_CALL_ARGS(string, append, txt);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * string(CMAP_STRING * string, const char * aisle)
{
  CMAP_STRING * string_ret = CMAP_STRING(CMAP_CALL(string, val), 0, aisle);
  CMAP_CALL_ARGS(cmap_kernel_public.pool_string(), release, string);
  return (CMAP_MAP *)string_ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * process(CMAP_MAP * map, const char * fn_name,
  CMAP_LIST * args)
{
  CMAP_FN * fn = (CMAP_FN *)CMAP_GET(map, fn_name);
  CMAP_MAP * ret = CMAP_FN_PROC(fn, map, args);
  CMAP_CALL_ARGS(cmap_kernel_public.pool_list(), release, args);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  $$,
  name,
  path,
  set_local,
  set_global,
  set_path,
  args,
  args_push,
  args_map,
  args_map_push,
  map,
  map_args,
  list,
  list_args,
  string_scanner,
  scanner_append,
  string,
  process
};
