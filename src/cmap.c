
#include "cmap.h"

#include <stddef.h>
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

/*******************************************************************************
*******************************************************************************/

void cmap_bootstrap(int argc, char ** argv)
{
  cmap_kernel_bootstrap(argc, argv);
}

int cmap_main()
{
  return cmap_kernel_main();
}

void cmap_exit(int ret)
{
  cmap_kernel_exit(ret);
}

void cmap_fatal()
{
  cmap_kernel_fatal();
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
  return cmap_ptr_create(size, delete_ptr, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

char cmap_nature(CMAP_MAP * map)
{
  if(map == NULL) return -1;
  else return CMAP_NATURE(map);
}

void cmap_set(CMAP_MAP * map, const char * key, CMAP_MAP * val,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_SET(map, key, val, proc_ctx);
}

CMAP_MAP * cmap_get(CMAP_MAP * map, const char * key)
{
  return CMAP_GET(map, key);
}

char cmap_has(CMAP_MAP * map, const char * key)
{
  return cmap_map_has(map, key);
}

/*******************************************************************************
*******************************************************************************/

void cmap_set_w_map(CMAP_MAP * map, CMAP_MAP * what, CMAP_MAP * val,
  CMAP_PROC_CTX * proc_ctx)
{
  unsigned char what_nature = CMAP_NATURE(what);
  if((CMAP_NATURE(map) == CMAP_LIST_NATURE) && (what_nature == CMAP_INT_NATURE))
  {
    int64_t i = cmap_int_get((CMAP_INT *)what);
    CMAP_LIST_SET((CMAP_LIST *)map, i, val, proc_ctx);
  }
  else if(what_nature == CMAP_STRING_NATURE)
  {
    const char * key = cmap_string_val((CMAP_STRING *)what);
    CMAP_SET(map, key, val, proc_ctx);
  }
}

CMAP_MAP * cmap_get_w_map(CMAP_MAP * map, CMAP_MAP * what)
{
  unsigned char what_nature = CMAP_NATURE(what);
  if((CMAP_NATURE(map) == CMAP_LIST_NATURE) && (what_nature == CMAP_INT_NATURE))
  {
    int64_t i = cmap_int_get((CMAP_INT *)what);
    return CMAP_LIST_GET((CMAP_LIST *)map, i);
  }
  else if(what_nature == CMAP_STRING_NATURE)
  {
    const char * key = cmap_string_val((CMAP_STRING *)what);
    return CMAP_GET(map, key);
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_copy_map(CMAP_MAP * dst, CMAP_MAP * src,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_util_copy(dst, src, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

int64_t cmap_cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  return cmap_cmp_run(map_l, map_r);
}

char cmap_true(CMAP_MAP * map)
{
  return cmap_cmp_is_true(map);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_add(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_add(map_l, map_r, proc_ctx);
}

CMAP_MAP * cmap_sub(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_sub(map_l, map_r, proc_ctx);
}

CMAP_MAP * cmap_mul(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_mul(map_l, map_r, proc_ctx);
}

CMAP_MAP * cmap_div(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  return cmap_op_div(map_l, map_r, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_add_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_add_self(map_dst, map_src);
}

void cmap_sub_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_sub_self(map_dst, map_src);
}

void cmap_mul_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_mul_self(map_dst, map_src);
}

void cmap_div_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  cmap_op_div_self(map_dst, map_src);
}

/*******************************************************************************
*******************************************************************************/

void cmap_inc(CMAP_MAP * map)
{
  cmap_op_inc(map);
}

void cmap_dec(CMAP_MAP * map)
{
  cmap_op_dec(map);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_lnew(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  CMAP_LIST * args)
{
  return cmap_fn_new(prototype, args, proc_ctx);
}

static CMAP_MAP * cmap_vnew(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  va_list args)
{
  CMAP_POOL_LIST_GHOST * pool = cmap_proc_ctx_pool_list_ghost(proc_ctx);
  CMAP_LIST * args_list = cmap_pool_list_ghost_take(pool, proc_ctx);
  cmap_util_vfill_list(args_list, args);

  CMAP_MAP * ret = cmap_lnew(prototype, proc_ctx, args_list);

  cmap_pool_list_ghost_release(pool, args_list, proc_ctx);

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
  return cmap_fn_process(fn, proc_ctx, map, args);
}

static CMAP_MAP * cmap_vfn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, va_list args)
{
  CMAP_POOL_LIST_GHOST * pool = cmap_proc_ctx_pool_list_ghost(proc_ctx);
  CMAP_LIST * args_list = cmap_pool_list_ghost_take(pool, proc_ctx);
  cmap_util_vfill_list(args_list, args);

  CMAP_MAP * ret = cmap_lfn_proc(fn, proc_ctx, map, args_list);

  cmap_pool_list_ghost_release(pool, args_list, proc_ctx);

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
  CMAP_MAP * fn = CMAP_GET(map, key);
  if((fn != NULL) && (CMAP_NATURE(fn) == CMAP_FN_NATURE))
    return cmap_lfn_proc((CMAP_FN *)fn, proc_ctx, map, args);
  else return map;
}

static CMAP_MAP * vproc(CMAP_MAP * map, const char * key,
  CMAP_PROC_CTX * proc_ctx, va_list args)
{
  CMAP_POOL_LIST_GHOST * pool = cmap_proc_ctx_pool_list_ghost(proc_ctx);
  CMAP_LIST * args_list = cmap_pool_list_ghost_take(pool, proc_ctx);
  cmap_util_vfill_list(args_list, args);

  CMAP_MAP * ret = cmap_lproc(map, key, proc_ctx, args_list);

  cmap_pool_list_ghost_release(pool, args_list, proc_ctx);

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
  CMAP_LIST * list = cmap_util_vto_list(proc_ctx, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_to_map(CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list key_maps;
  va_start(key_maps, proc_ctx);
  CMAP_MAP * map = cmap_util_vto_map(proc_ctx, key_maps);
  va_end(key_maps);
  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_ENV * cmap_env()
{
  return cmap_env_create();
}

void cmap_env_main(CMAP_ENV * env, CMAP_ENV_MAIN main_)
{
  cmap_env_set_main(env, main_);
}

/*******************************************************************************
*******************************************************************************/

CMAP_PROC_CTX * cmap_proc_ctx(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_proc_ctx_new(proc_ctx);
}

CMAP_MAP * cmap_delete_proc_ctx(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * ret)
{
  return cmap_proc_ctx_delete(proc_ctx, ret);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_global_env(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_proc_ctx_global_env(proc_ctx);
}

CMAP_MAP * cmap_definitions(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_proc_ctx_local_definitions(proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void * cmap_alloc(int size)
{
  return cmap_mem_alloc(size);
}

void cmap_free(void * ptr)
{
  cmap_mem_free(ptr);
}
