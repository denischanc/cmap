
#include "cmap-proc-ctx.h"

#include "cmap.h"
#include "cmap-util.h"
#include "cmap-list.h"
#include "cmap-kernel.h"
#include "cmap-env.h"
#include "cmap-ptr.h"
#include "cmap-lifecycle.h"

/*******************************************************************************
*******************************************************************************/

static const char * AISLE_SCANNER = "cmap-internal-scanner";
static const char * AISLE_LOCAL_STACK = "cmap-internal-local-stack";
static const char * AISLE_DEFINITIONS = "cmap-internal-definitions";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_ENV * env;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static CMAP_ENV * env(CMAP_PROC_CTX * this)
{
  return ((INTERNAL *)this -> internal) -> env;
}

static CMAP_AISLESTORE * aislestore(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL(env_, aislestore);
}

static CMAP_POOL_LIST * pool_list(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, pool_list, this);
}

static CMAP_POOL_STRING * pool_string(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, pool_string, this);
}

static CMAP_POOL_INT * pool_int(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, pool_int, this);
}

static CMAP_MAP * global_env(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, global, this);
}

/*******************************************************************************
*******************************************************************************/

static void delete_scanner(void * ptr)
{
  cmap_parser_lex_destroy(*(yyscan_t *)ptr);
}

static void push_scanner(CMAP_PROC_CTX * this)
{
  CMAP_PTR * scanner = cmap_ptr_public.create(
    sizeof(yyscan_t), delete_scanner, this, AISLE_SCANNER);
  cmap_parser_lex_init_extra(this, (yyscan_t *)CMAP_CALL(scanner, get));
}

static void pop_scanner(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  CMAP_CALL_ARGS(as, delete_last, AISLE_SCANNER);
}

static yyscan_t scanner(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  CMAP_PTR * scanner = (CMAP_PTR *)CMAP_GET(as, AISLE_SCANNER);
  if(scanner == NULL) return NULL;
  else return *(yyscan_t *)CMAP_CALL(scanner, get);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIFECYCLE * delete_local_stack(CMAP_LIFECYCLE * lc)
{
  CMAP_LIST * local_stack = (CMAP_LIST *)lc;
  cmap_util_public.delete_list_vals(local_stack);
  return cmap_list_public.delete(local_stack);
}

static void push_local_stack(CMAP_PROC_CTX * this)
{
  CMAP_LIST * local_stack = CMAP_LIST(0, this, AISLE_LOCAL_STACK);
  local_stack -> super.super.delete = delete_local_stack;
}

static void pop_local_stack(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  CMAP_CALL_ARGS(as, delete_last, AISLE_LOCAL_STACK);
}

static CMAP_LIST * local_stack(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  return (CMAP_LIST *)CMAP_GET(as, AISLE_LOCAL_STACK);
}

/*******************************************************************************
*******************************************************************************/

static void push_definitions(CMAP_PROC_CTX * this)
{
  cmap_map_public.create_root(this, AISLE_DEFINITIONS);
}

static void pop_definitions(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  CMAP_CALL_ARGS(as, delete_last, AISLE_DEFINITIONS);
}

static CMAP_MAP * definitions(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  return CMAP_GET(as, AISLE_DEFINITIONS);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PROC_CTX * this)
{
  CMAP_KERNEL_FREE(this -> internal);
  CMAP_KERNEL_FREE(this);
}

static CMAP_PROC_CTX * create(CMAP_ENV * env_)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> env = env_;

  CMAP_KERNEL_ALLOC_PTR(proc_ctx, CMAP_PROC_CTX);
  proc_ctx -> internal = internal;
  proc_ctx -> delete = delete;
  proc_ctx -> env = env;
  proc_ctx -> aislestore = aislestore;
  proc_ctx -> pool_list = pool_list;
  proc_ctx -> pool_string = pool_string;
  proc_ctx -> pool_int = pool_int;
  proc_ctx -> global_env = global_env;
  proc_ctx -> push_scanner = push_scanner;
  proc_ctx -> pop_scanner = pop_scanner;
  proc_ctx -> scanner = scanner;
  proc_ctx -> push_local_stack = push_local_stack;
  proc_ctx -> pop_local_stack = pop_local_stack;
  proc_ctx -> local_stack = local_stack;
  proc_ctx -> push_definitions = push_definitions;
  proc_ctx -> pop_definitions = pop_definitions;
  proc_ctx -> definitions = definitions;

  return proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public =
{
  create
};
