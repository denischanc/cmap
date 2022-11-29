
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-fn.h"
#include "cmap-parser.h"
#include "cmap-pool.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proc_impl(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * definitions,
  const char * impl)
{
  if((impl == NULL) || !strcmp(impl, "")) return NULL;

  CMAP_CALL_ARGS(proc_ctx, push_definitions, definitions);
  CMAP_CALL(proc_ctx, push_scanner);

  yyscan_t scanner = CMAP_CALL(proc_ctx, scanner);
  cmap_parser_set_in(fmemopen((void *)impl, strlen(impl), "r"), scanner);
  cmap_parser_parse(scanner, proc_ctx);
  fclose(cmap_parser_get_in(scanner));

  CMAP_CALL(proc_ctx, pop_scanner);
  CMAP_CALL(proc_ctx, pop_definitions);

  return proc_ctx -> ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * name(CMAP_PROC_CTX * proc_ctx, const char * name)
{
  CMAP_MAP * map = NULL;
  CMAP_MAP * definitions = CMAP_CALL(proc_ctx, definitions);

  if((definitions != NULL) && CMAP_CALL_ARGS(definitions, is_key, name))
    map = definitions;
  else map = CMAP_CALL(proc_ctx, global_env);

  CMAP_MAP * ret = CMAP_GET(map, name);
  CMAP_KERNEL_FREE(name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * path(CMAP_MAP * map, const char * name)
{
  CMAP_MAP * ret = CMAP_GET(map, name);
  CMAP_KERNEL_FREE(name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void set_local(CMAP_PROC_CTX * proc_ctx, const char * name,
  CMAP_MAP * map)
{
  CMAP_MAP * definitions = CMAP_CALL(proc_ctx, require_definitions);
  CMAP_SET(definitions, name, map);
  CMAP_KERNEL_FREE(name);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(CMAP_PROC_CTX * proc_ctx, const char * name,
  CMAP_MAP * map)
{
  CMAP_MAP * global_env = CMAP_CALL(proc_ctx, global_env);
  CMAP_SET(global_env, name, map);
  CMAP_KERNEL_FREE(name);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(CMAP_MAP * src, const char * name, CMAP_MAP * map)
{
  CMAP_SET(src, name, map);
  CMAP_KERNEL_FREE(name);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * args_empty(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  return CMAP_CALL_ARGS(pool, take, proc_ctx);
}

static CMAP_LIST * args_push(CMAP_LIST * list, CMAP_MAP * map)
{
  return CMAP_LIST_PUSH(list, map);
}

static CMAP_LIST * args(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map)
{
  return args_push(args_empty(proc_ctx), map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * args_map_push(CMAP_PROC_CTX * proc_ctx, CMAP_LIST * list,
  const char * name, CMAP_MAP * map)
{
  CMAP_POOL_STRING * pool = CMAP_CALL(proc_ctx, pool_string);
  CMAP_STRING * name_s = CMAP_CALL_ARGS(pool, take, proc_ctx);
  CMAP_CALL_ARGS(name_s, append, name);
  CMAP_KERNEL_FREE(name);
  CMAP_LIST_PUSH(list, name_s);
  return CMAP_LIST_PUSH(list, map);
}

static CMAP_LIST * args_map(CMAP_PROC_CTX * proc_ctx, const char * name,
  CMAP_MAP * map)
{
  return args_map_push(proc_ctx, args_empty(proc_ctx), name, map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * map_args(CMAP_LIST * args, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_MAP * map = CMAP_MAP(proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);

  CMAP_POOL_STRING * pool_string = CMAP_CALL(proc_ctx, pool_string);
  CMAP_STRING * key;
  while((key = (CMAP_STRING *)CMAP_LIST_SHIFT(args)) != NULL)
  {
    CMAP_MAP * val = CMAP_LIST_SHIFT(args);
    CMAP_SET(map, CMAP_CALL(key, val), val);
    CMAP_CALL_ARGS(pool_string, release, key);
  }

  CMAP_POOL_LIST * pool_list = CMAP_CALL(proc_ctx, pool_list);
  CMAP_CALL_ARGS(pool_list, release, args);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * list_args(CMAP_LIST * args, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_LIST * list = CMAP_LIST(0, proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);

  int size = CMAP_CALL(args, size);
  for(int i = 0; i < size; i++) CMAP_LIST_PUSH(list, CMAP_LIST_SHIFT(args));

  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  CMAP_CALL_ARGS(pool, release, args);

  return (CMAP_MAP *)list;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRING * string_scanner(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_POOL_STRING * pool = CMAP_CALL(proc_ctx, pool_string);
  return CMAP_CALL_ARGS(pool, take, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static void scanner_append(CMAP_STRING * string, const char * txt)
{
  CMAP_CALL_ARGS(string, append, txt);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * string(CMAP_STRING * string, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_STRING * string_ret = CMAP_STRING(CMAP_CALL(string, val), 0,
    proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);
  CMAP_POOL_STRING * pool = CMAP_CALL(proc_ctx, pool_string);
  CMAP_CALL_ARGS(pool, release, string);
  return (CMAP_MAP *)string_ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_FN * get_fn(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, const char * fn_name)
{
  CMAP_FN * fn;
  if(map == NULL) fn = (CMAP_FN *)name(proc_ctx, fn_name);
  else
  {
    fn = (CMAP_FN *)CMAP_GET(map, fn_name);
    CMAP_KERNEL_FREE(fn_name);
  }
  return fn;
}

static CMAP_MAP * process(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, const char * fn_name, CMAP_LIST * args)
{
  CMAP_FN * fn = get_fn(proc_ctx, map, fn_name);
  CMAP_MAP * ret = CMAP_FN_PROC(fn, proc_ctx, map, args);

  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  CMAP_CALL_ARGS(pool, release, args);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * new(CMAP_MAP * map, const char * fn_name, CMAP_LIST * args,
  CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_FN * fn = get_fn(proc_ctx, map, fn_name);
  CMAP_MAP * ret = CMAP_CALL_ARGS(fn, new, args, proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);

  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  CMAP_CALL_ARGS(pool, release, args);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proc_impl_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * definitions = CMAP_CALL(proc_ctx, definitions);
  CMAP_STRING * impl = (CMAP_STRING *)CMAP_GET(definitions, "cmap-fn-impl");
  return proc_impl(proc_ctx, definitions, CMAP_CALL(impl, val));
}

static CMAP_FN * fn_with_impl(CMAP_STRING * impl, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_FN * fn = CMAP_FN(proc_impl_fn, proc_ctx, aisle);

  CMAP_MAP * definitions = CMAP_CALL_ARGS(fn, require_definitions, proc_ctx);
  const char * impl_ = CMAP_CALL(impl, val);
  CMAP_SET(definitions, "cmap-fn-impl", CMAP_STRING(impl_, 0, proc_ctx, aisle));

  CMAP_KERNEL_FREE(aisle);

  cmap_util_public.copy(definitions, CMAP_CALL(proc_ctx, definitions));

  return fn;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  proc_impl,
  name,
  path,
  set_local,
  set_global,
  set_path,
  args_empty,
  args_push,
  args,
  args_map_push,
  args_map,
  map_args,
  list_args,
  string_scanner,
  scanner_append,
  string,
  process,
  new,
  fn_with_impl
};
