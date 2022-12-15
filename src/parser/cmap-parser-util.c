
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-fn.h"
#include "cmap-pool.h"
#include "cmap-util.h"
#include "cmap-int.h"
#include "cmap-proc-ctx.h"
#include "cmap-scanner.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static const char * FN_IMPL_KEY = "cmap-fn-impl";
static const char * FN_ARGS_KEY = "cmap-fn-args";

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
  CMAP_MAP * definitions = cmap_definitions(proc_ctx);

  if((definitions != NULL) && CMAP_CALL_ARGS(definitions, is_key, name))
    map = definitions;
  else map = cmap_global_env(proc_ctx);

  CMAP_MAP * ret = cmap_get(map, name);
  CMAP_KERNEL_FREE(name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * path(CMAP_MAP * map, const char * name)
{
  CMAP_MAP * ret = cmap_get(map, name);
  CMAP_KERNEL_FREE(name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void set_local(CMAP_PROC_CTX * proc_ctx, const char * name,
  CMAP_MAP * map)
{
  CMAP_MAP * definitions = cmap_require_definitions(proc_ctx);
  cmap_set(definitions, name, map);
  CMAP_KERNEL_FREE(name);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(CMAP_PROC_CTX * proc_ctx, const char * name,
  CMAP_MAP * map)
{
  CMAP_MAP * global_env = cmap_global_env(proc_ctx);
  cmap_set(global_env, name, map);
  CMAP_KERNEL_FREE(name);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(CMAP_MAP * src, const char * name, CMAP_MAP * map)
{
  cmap_set(src, name, map);
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

static CMAP_LIST * args_name_push(CMAP_PROC_CTX * proc_ctx, CMAP_LIST * list,
  const char * name)
{
  CMAP_POOL_STRING * pool = CMAP_CALL(proc_ctx, pool_string);
  CMAP_STRING * name_s = CMAP_CALL_ARGS(pool, take, proc_ctx);
  CMAP_CALL_ARGS(name_s, append, name);
  CMAP_KERNEL_FREE(name);
  return CMAP_LIST_PUSH(list, name_s);
}

static CMAP_LIST * args_name(CMAP_PROC_CTX * proc_ctx, const char * name)
{
  return args_name_push(proc_ctx, args_empty(proc_ctx), name);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * args_map_push(CMAP_PROC_CTX * proc_ctx, CMAP_LIST * list,
  const char * name, CMAP_MAP * map)
{
  args_name_push(proc_ctx, list, name);
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
  CMAP_MAP * map = cmap_map(proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);

  CMAP_POOL_STRING * pool_string = CMAP_CALL(proc_ctx, pool_string);
  CMAP_STRING * key;
  while((key = (CMAP_STRING *)CMAP_LIST_SHIFT(args)) != NULL)
  {
    CMAP_MAP * val = CMAP_LIST_SHIFT(args);
    cmap_set(map, cmap_string_val(key), val);
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
  CMAP_LIST * list = cmap_list(0, proc_ctx, aisle);
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
  CMAP_STRING * string_ret = cmap_string(cmap_string_val(string), 0,
    proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);
  CMAP_POOL_STRING * pool = CMAP_CALL(proc_ctx, pool_string);
  CMAP_CALL_ARGS(pool, release, string);
  return (CMAP_MAP *)string_ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * int_(int64_t i, CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_MAP * ret = (CMAP_MAP *)cmap_int(i, proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);
  return ret;
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
    fn = (CMAP_FN *)cmap_get(map, fn_name);
    CMAP_KERNEL_FREE(fn_name);
  }
  return fn;
}

static CMAP_MAP * process(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, const char * fn_name, CMAP_LIST * args)
{
  CMAP_FN * fn = get_fn(proc_ctx, map, fn_name);
  CMAP_MAP * ret = cmap_lfn_proc(fn, proc_ctx, map, args);

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
  CMAP_MAP * ret = cmap_lnew(fn, proc_ctx, aisle, args);
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
  CMAP_MAP * definitions = cmap_definitions(proc_ctx);

  cmap_set(definitions, "this", map);
  cmap_set(definitions, "args", (CMAP_MAP *)args);

  CMAP_LIST * args_name = (CMAP_LIST *)cmap_get(definitions, FN_ARGS_KEY);
  int size = CMAP_CALL(args_name, size);
  for(int i = 0; i < size; i++)
  {
    CMAP_STRING * arg = (CMAP_STRING *)cmap_list_get(args_name, i);
    cmap_set(definitions, cmap_string_val(arg), cmap_list_get(args, i));
  }

  CMAP_STRING * impl = (CMAP_STRING *)cmap_get(definitions, FN_IMPL_KEY);
  return proc_impl(proc_ctx, definitions, cmap_string_val(impl));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * function_delete(CMAP_MAP * map)
{
  CMAP_FN * fn = (CMAP_FN *)map;
  CMAP_MAP * definitions = CMAP_CALL_ARGS(fn, require_definitions, NULL);

  CMAP_MAP * impl = cmap_get(definitions, FN_IMPL_KEY);
  cmap_delete(impl);

  CMAP_LIST * args = (CMAP_LIST *)cmap_get(definitions, FN_ARGS_KEY);
  cmap_util_public.delete_list_vals(args);
  cmap_delete((CMAP_MAP *)args);

  return cmap_fn_public.delete(fn);
}

static CMAP_MAP * function(CMAP_PROC_CTX * proc_ctx, const char * aisle,
  CMAP_LIST * args, CMAP_STRING * impl)
{
  CMAP_POOL_STRING * pool_string = CMAP_CALL(proc_ctx, pool_string);
  CMAP_POOL_LIST * pool_list = CMAP_CALL(proc_ctx, pool_list);

  CMAP_FN * fn = cmap_fn(proc_impl_fn, proc_ctx, aisle);
  CMAP_KERNEL_FREE(aisle);
  ((CMAP_MAP *)fn) -> delete = function_delete;
  CMAP_MAP * definitions = CMAP_CALL_ARGS(fn, require_definitions, proc_ctx);

  cmap_set(definitions, FN_IMPL_KEY,
    (CMAP_MAP *)cmap_string(cmap_string_val(impl), 0, proc_ctx, NULL));
  CMAP_CALL_ARGS(pool_string, release, impl);

  CMAP_LIST * args_ = cmap_list(0, proc_ctx, NULL);
  CMAP_STRING * arg;
  while((arg = (CMAP_STRING *)CMAP_LIST_SHIFT(args)) != NULL)
  {
    CMAP_LIST_PUSH(args_, cmap_string(cmap_string_val(arg), 0, proc_ctx, NULL));
    CMAP_CALL_ARGS(pool_string, release, arg);
  }
  CMAP_CALL_ARGS(pool_list, release, args);
  cmap_set(definitions, FN_ARGS_KEY, (CMAP_MAP *)args_);

  cmap_util_public.copy(definitions, cmap_definitions(proc_ctx));

  return (CMAP_MAP *)fn;
}

/*******************************************************************************
*******************************************************************************/

#define CMP_IMPL(name, op) \
static CMAP_MAP * name(CMAP_MAP * map_l, CMAP_MAP * map_r) \
{ \
  return NULL; \
}

CMAP_PARSER_UTIL_CMP_LOOP(CMP_IMPL)

/*******************************************************************************
*******************************************************************************/

#define CMP_SET(name, op) name,

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
  args_name_push,
  args_name,
  args_map_push,
  args_map,
  map_args,
  list_args,
  string_scanner,
  scanner_append,
  string,
  int_,
  process,
  new,
  function,
  CMAP_PARSER_UTIL_CMP_LOOP(CMP_SET)
};
