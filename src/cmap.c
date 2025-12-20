
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
#include "cmap-env.h"
#include "cmap-kernel.h"
#include "cmap-proc-ctx.h"
#include "cmap-cmp.h"
#include "cmap-op.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

CMAP_CONFIG * cmap_config()
{
  return cmap_config_public.instance();
}

void cmap_bootstrap(int argc, char ** argv)
{
  cmap_kernel_public.bootstrap(argc, argv);
}

int cmap_main()
{
  return cmap_kernel_public.instance() -> main();
}

void cmap_exit(int ret)
{
  cmap_kernel_public.instance() -> exit(ret);
}

void cmap_fatal()
{
  cmap_kernel_public.instance() -> fatal();
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_MAP(proc_ctx);
}

CMAP_LIST * cmap_list(int size_inc, CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_LIST(size_inc, proc_ctx);
}

CMAP_FN * cmap_fn(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_FN(process, proc_ctx);
}

CMAP_STRING * cmap_string(const char * val, int size_inc,
  CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_STRING(val, size_inc, proc_ctx);
}

CMAP_INT * cmap_int(int64_t val, CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_INT(val, proc_ctx);
}

CMAP_DOUBLE * cmap_double(double val, CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_DOUBLE(val, proc_ctx);
}

CMAP_PTR * cmap_ptr(int size, CMAP_PTR_DELETE delete_ptr,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_ptr_public.create(size, delete_ptr, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_nature(CMAP_MAP * map)
{
  if(map == NULL) return NULL;
  else return CMAP_NATURE(map);
}

void cmap_set(CMAP_MAP * map, const char * key, CMAP_MAP * val)
{
  CMAP_SET(map, key, val);
}

CMAP_MAP * cmap_get(CMAP_MAP * map, const char * key)
{
  return CMAP_GET(map, key);
}

char cmap_is_key(CMAP_MAP * map, const char * key)
{
  return CMAP_CALL_ARGS(map, is_key, key);
}

/*******************************************************************************
*******************************************************************************/

void cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val)
{
  CMAP_LIST_SET(list, i, val);
}

CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i)
{
  return CMAP_LIST_GET(list, i);
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

void ** cmap_ptr_ref(CMAP_PTR * ptr)
{
  return CMAP_CALL(ptr, ref);
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

void cmap_set_w_map(CMAP_MAP * map, CMAP_MAP * what, CMAP_MAP * val)
{
  const char * what_nature = CMAP_NATURE(what);
  if((CMAP_NATURE(map) == CMAP_LIST_NATURE) && (what_nature == CMAP_INT_NATURE))
  {
    int64_t i = CMAP_CALL((CMAP_INT *)what, get);
    CMAP_LIST_SET((CMAP_LIST *)map, i, val);
  }
  else if(what_nature == CMAP_STRING_NATURE)
  {
    const char * key = CMAP_CALL((CMAP_STRING *)what, val);
    CMAP_SET(map, key, val);
  }
}

CMAP_MAP * cmap_get_w_map(CMAP_MAP * map, CMAP_MAP * what)
{
  const char * what_nature = CMAP_NATURE(what);
  if((CMAP_NATURE(map) == CMAP_LIST_NATURE) && (what_nature == CMAP_INT_NATURE))
  {
    int64_t i = CMAP_CALL((CMAP_INT *)what, get);
    return CMAP_LIST_GET((CMAP_LIST *)map, i);
  }
  else if(what_nature == CMAP_STRING_NATURE)
  {
    const char * key = CMAP_CALL((CMAP_STRING *)what, val);
    return CMAP_GET(map, key);
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_copy_map(CMAP_MAP * dst, CMAP_MAP * src)
{
  return cmap_util_public.copy(dst, src);
}

/*******************************************************************************
*******************************************************************************/

int64_t cmap_cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  return cmap_cmp_public.cmp(map_l, map_r);
}

char cmap_true(CMAP_MAP * map)
{
  return cmap_cmp_public.is_true(map);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_add(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_public.add(map_l, map_r, proc_ctx);
}

CMAP_MAP * cmap_sub(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_public.sub(map_l, map_r, proc_ctx);
}

CMAP_MAP * cmap_mul(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_public.mul(map_l, map_r, proc_ctx);
}

CMAP_MAP * cmap_div(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_public.div(map_l, map_r, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_add_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_public.add_self(map_dst, map_src);
}

void cmap_sub_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_public.sub_self(map_dst, map_src);
}

void cmap_mul_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_public.mul_self(map_dst, map_src);
}

void cmap_div_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_public.div_self(map_dst, map_src);
}

/*******************************************************************************
*******************************************************************************/

void cmap_inc(CMAP_MAP * map)
{
  cmap_op_public.inc(map);
}

void cmap_dec(CMAP_MAP * map)
{
  cmap_op_public.dec(map);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_lnew(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  CMAP_LIST * args)
{
  return CMAP_CALL_ARGS(prototype, new, args, proc_ctx);
}

static CMAP_MAP * cmap_vnew(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  va_list args)
{
  CMAP_POOL_LIST_GHOST * pool = CMAP_CALL(proc_ctx, pool_list_ghost);
  CMAP_LIST * args_list = CMAP_CALL_ARGS(pool, take, proc_ctx);
  cmap_util_public.vfill_list(args_list, args);

  CMAP_MAP * ret = cmap_lnew(prototype, proc_ctx, args_list);

  CMAP_CALL_ARGS(pool, release, args_list);

  return ret;
}

CMAP_MAP * cmap_new(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list args;
  va_start(args, proc_ctx);
  CMAP_MAP * ret = cmap_vnew(prototype, proc_ctx, args);
  va_end(args);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_lfn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  if(fn != NULL) return CMAP_CALL_ARGS(fn, process, proc_ctx, map, args);
  else return map;
}

static CMAP_MAP * cmap_vfn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, va_list args)
{
  CMAP_POOL_LIST_GHOST * pool = CMAP_CALL(proc_ctx, pool_list_ghost);
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
  CMAP_POOL_LIST_GHOST * pool = CMAP_CALL(proc_ctx, pool_list_ghost);
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

CMAP_LIST * cmap_to_list(CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list maps;
  va_start(maps, proc_ctx);
  CMAP_LIST * list = cmap_util_public.vto_list(proc_ctx, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_to_map(CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list key_maps;
  va_start(key_maps, proc_ctx);
  CMAP_MAP * map = cmap_util_public.vto_map(proc_ctx, key_maps);
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

CMAP_ENV * cmap_env()
{
  return cmap_env_public.create();
}

void cmap_env_main(CMAP_ENV * env, void (*init)(CMAP_PROC_CTX *))
{
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(env);
  init(proc_ctx);
  CMAP_CALL_ARGS(proc_ctx, delete, NULL);
}

/*******************************************************************************
*******************************************************************************/

CMAP_PROC_CTX * cmap_proc_ctx(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_CALL(proc_ctx, new_level);
}

CMAP_MAP * cmap_delete_proc_ctx(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * ret)
{
  return CMAP_CALL_ARGS(proc_ctx, delete, ret);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_global_env(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_CALL(proc_ctx, global_env);
}

CMAP_MAP * cmap_definitions(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_CALL(proc_ctx, local_definitions);
}
