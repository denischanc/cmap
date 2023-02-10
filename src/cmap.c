
#include "cmap.h"

#include <stdlib.h>
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-fn.h"
#include "cmap-string.h"
#include "cmap-int.h"
#include "cmap-double.h"
#include "cmap-ptr.h"
#include "cmap-mem.h"
#include "cmap-util.h"
#include "cmap-parser-util.h"
#include "cmap-env.h"
#include "cmap-kernel.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

void cmap_bootstrap(CMAP_KERNEL_CFG * cfg)
{
  cmap_kernel_public.bootstrap(cfg);
}

int cmap_main()
{
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  if(kernel == NULL) cmap_fatal();
  return kernel -> main();
}

void cmap_exit(int ret)
{
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  if(kernel != NULL) kernel -> exit(ret);
  else exit(ret);
}

void cmap_fatal()
{
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  if(kernel != NULL) kernel -> fatal();
  else exit(EXIT_FAILURE);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map(CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  return cmap_map_public.create(proc_ctx, aisle);
}

CMAP_LIST * cmap_list(int size_inc, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  return cmap_list_public.create(size_inc, proc_ctx, aisle);
}

CMAP_FN * cmap_fn(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  return cmap_fn_public.create(process, proc_ctx, aisle);
}

CMAP_STRING * cmap_string(const char * val, int size_inc,
  CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  return cmap_string_public.create(val, size_inc, proc_ctx, aisle);
}

CMAP_INT * cmap_int(int64_t val, CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  return cmap_int_public.create(val, proc_ctx, aisle);
}

CMAP_DOUBLE * cmap_double(double val, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  return cmap_double_public.create(val, proc_ctx, aisle);
}

CMAP_PTR * cmap_ptr(int size, CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  return cmap_ptr_public.create(size, proc_ctx, aisle);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_nature(CMAP_MAP * map)
{
  return CMAP_CALL(map, nature);
}

CMAP_MAP * cmap_delete(CMAP_MAP * map)
{
  return CMAP_CALL(map, delete);
}

void cmap_set(CMAP_MAP * map, const char * key, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(map, set, key, val);
}

CMAP_MAP * cmap_get(CMAP_MAP * map, const char * key)
{
  return CMAP_CALL_ARGS(map, get, key);
}

/*******************************************************************************
*******************************************************************************/

void cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(list, set, i, val);
}

CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i)
{
  return CMAP_CALL_ARGS(list, get, i);
}

/*******************************************************************************
*******************************************************************************/

void cmap_int_set(CMAP_INT * i, int64_t val)
{
  CMAP_CALL_ARGS(i, set, val);
}

int64_t cmap_int_get(CMAP_INT * i)
{
  return CMAP_CALL(i, get);
}

/*******************************************************************************
*******************************************************************************/

void cmap_double_set(CMAP_DOUBLE * d, double val)
{
  CMAP_CALL_ARGS(d, set, val);
}

double cmap_double_get(CMAP_DOUBLE * d)
{
  return CMAP_CALL(d, get);
}

/*******************************************************************************
*******************************************************************************/

void * cmap_ptr_get(CMAP_PTR * ptr)
{
  return CMAP_CALL(ptr, get);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_string_val(CMAP_STRING * string)
{
  return CMAP_CALL(string, val);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fn_require_definitions(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_CALL_ARGS(fn, require_definitions, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_lnew(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  const char * aisle, CMAP_LIST * args)
{
  return CMAP_CALL_ARGS(prototype, new, args, proc_ctx, aisle);
}

static CMAP_MAP * cmap_vnew(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  const char * aisle, va_list args)
{
  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  CMAP_LIST * args_list = CMAP_CALL_ARGS(pool, take, proc_ctx);
  cmap_util_public.vfill_list(args_list, args);

  CMAP_MAP * ret = cmap_lnew(prototype, proc_ctx, aisle, args_list);

  CMAP_CALL_ARGS(pool, release, args_list);

  return ret;
}

CMAP_MAP * cmap_new(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  const char * aisle, ...)
{
  va_list args;
  va_start(args, aisle);
  CMAP_MAP * ret = cmap_vnew(prototype, proc_ctx, aisle, args);
  va_end(args);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_lfn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  return CMAP_CALL_ARGS(fn, process, proc_ctx, map, args);
}

static CMAP_MAP * cmap_vfn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, va_list args)
{
  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  CMAP_LIST * args_list = CMAP_CALL_ARGS(pool, take, proc_ctx);
  cmap_util_public.vfill_list(args_list, args);

  CMAP_MAP * ret = cmap_lfn_proc(fn, proc_ctx, map, args_list);

  CMAP_CALL_ARGS(pool, release, args_list);

  return ret;
}

CMAP_MAP * cmap_fn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  ...)
{
  va_list args;
  va_start(args, map);
  CMAP_MAP * ret = cmap_vfn_proc(fn, proc_ctx, map, args);
  va_end(args);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_lproc(CMAP_MAP * map, const char * key,
  CMAP_PROC_CTX * proc_ctx, CMAP_LIST * args)
{
  CMAP_MAP * ret = NULL;

  CMAP_MAP * fn_tmp = CMAP_GET(map, key);
  if((fn_tmp != NULL) && (CMAP_NATURE(fn_tmp) == CMAP_FN_NATURE))
  {
    CMAP_FN * fn = (CMAP_FN *)fn_tmp;
    ret = cmap_lfn_proc(fn, proc_ctx, map, args);
  }

  return ret;
}

static CMAP_MAP * vproc(CMAP_MAP * map, const char * key,
  CMAP_PROC_CTX * proc_ctx, va_list args)
{
  CMAP_POOL_LIST * pool = CMAP_CALL(proc_ctx, pool_list);
  CMAP_LIST * args_list = CMAP_CALL_ARGS(pool, take, proc_ctx);
  cmap_util_public.vfill_list(args_list, args);

  CMAP_MAP * ret = cmap_lproc(map, key, proc_ctx, args_list);

  CMAP_CALL_ARGS(pool, release, args_list);

  return ret;
}

CMAP_MAP * cmap_proc(CMAP_MAP * map, const char * key,
  CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list args;
  va_start(args, proc_ctx);
  CMAP_MAP * ret = vproc(map, key, proc_ctx, args);
  va_end(args);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_to_list(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...)
{
  va_list maps;
  va_start(maps, aisle);
  CMAP_LIST * list = cmap_util_public.vto_list(proc_ctx, aisle, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_to_map(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...)
{
  va_list key_maps;
  va_start(key_maps, aisle);
  CMAP_MAP * map = cmap_util_public.vto_map(proc_ctx, aisle, key_maps);
  va_end(key_maps);
  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MEM_STATE * cmap_mem_state()
{
  return cmap_mem_public.state();
}

/*******************************************************************************
*******************************************************************************/

void cmap_delete_aisle(CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_AISLESTORE * as = CMAP_CALL(proc_ctx, aislestore);
  CMAP_CALL_ARGS(as, delete, aisle);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_proc_impl(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * definitions,
  const char * impl)
{
  return cmap_parser_util_public.proc_impl(proc_ctx, definitions, impl);
}

/*******************************************************************************
*******************************************************************************/

CMAP_ENV * cmap_env()
{
  return cmap_env_public.create();
}

CMAP_MAP * cmap_env_main(CMAP_ENV * env, int argc, char * argv[],
  CMAP_MAP * definitions, const char * impl)
{
  return CMAP_CALL_ARGS(env, main, argc, argv, definitions, impl);
}

/*******************************************************************************
*******************************************************************************/

CMAP_PROC_CTX * cmap_proc_ctx(CMAP_ENV * env)
{
  return cmap_proc_ctx_public.create(env);
}

void cmap_delete_proc_ctx(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_CALL(proc_ctx, delete);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_global_env(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_CALL(proc_ctx, global_env);
}

CMAP_MAP * cmap_definitions(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_CALL(proc_ctx, definitions);
}

CMAP_MAP * cmap_require_definitions(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_CALL(proc_ctx, require_definitions);
}
