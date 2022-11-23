
#include "cmap-proc-ctx.h"

#include "cmap-common.h"
#include "cmap-util.h"
#include "cmap-list.h"
#include "cmap-kernel.h"
#include "cmap-env.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST * scanners, * local_stacks, * definitions_list;

  CMAP_ENV * env;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static void push_scanner(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> scanners == NULL)
    internal -> scanners = CMAP_LIST(0, this, NULL);

  yyscan_t scanner;
  cmap_parser_lex_init_extra(this, &scanner);
  CMAP_LIST_PUSH(internal -> scanners, scanner);
}

static void pop_scanner(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  yyscan_t scanner = (yyscan_t)CMAP_LIST_POP(internal -> scanners);
  cmap_parser_lex_destroy(scanner);
}

static yyscan_t scanner(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> scanners == NULL) return NULL;
  else
  {
    int size = CMAP_CALL(internal -> scanners, size);
    return (yyscan_t)CMAP_LIST_GET(internal -> scanners, size - 1);
  }
}

/*******************************************************************************
*******************************************************************************/

static void push_local_stack(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> local_stacks == NULL)
    internal -> local_stacks = CMAP_LIST(0, this, NULL);

  CMAP_LIST_PUSH(internal -> local_stacks, CMAP_LIST(0, this, NULL));
}

static void pop_local_stack(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_LIST * stack = (CMAP_LIST *)CMAP_LIST_POP(internal -> local_stacks);

  cmap_util_public.delete_list_vals(stack);
  CMAP_DELETE(stack);
}

static CMAP_LIST * local_stack(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> local_stacks == NULL) return NULL;
  else
  {
    int size = CMAP_CALL(internal -> local_stacks, size);
    return (CMAP_LIST *)CMAP_LIST_GET(internal -> local_stacks, size - 1);
  }
}

/*******************************************************************************
*******************************************************************************/

static void push_definitions(CMAP_PROC_CTX * this, CMAP_MAP * definitions)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> definitions_list == NULL)
    internal -> definitions_list = CMAP_LIST(0, this, NULL);

  CMAP_LIST_PUSH(internal -> definitions_list, definitions);
}

static void pop_definitions(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_LIST_POP(internal -> definitions_list);
}

static CMAP_MAP * definitions(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> definitions_list == NULL) return NULL;
  else
  {
    int size = CMAP_CALL(internal -> definitions_list, size);
    return CMAP_LIST_GET(internal -> definitions_list, size - 1);
  }
}

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

static CMAP_MAP * global_env(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, global, this);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> scanners != NULL) CMAP_DELETE(internal -> scanners);
  if(internal -> local_stacks != NULL) CMAP_DELETE(internal -> local_stacks);
  if(internal -> definitions_list != NULL)
    CMAP_DELETE(internal -> definitions_list);

  CMAP_KERNEL_FREE(internal);
  CMAP_KERNEL_FREE(this);
}

static CMAP_PROC_CTX * create(CMAP_ENV * env_)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> scanners = NULL;
  internal -> local_stacks = NULL;
  internal -> definitions_list = NULL;
  internal -> env = env_;

  CMAP_KERNEL_ALLOC_PTR(proc_ctx, CMAP_PROC_CTX);
  proc_ctx -> internal = internal;
  proc_ctx -> delete = delete;
  proc_ctx -> push_scanner = push_scanner;
  proc_ctx -> pop_scanner = pop_scanner;
  proc_ctx -> scanner = scanner;
  proc_ctx -> push_local_stack = push_local_stack;
  proc_ctx -> pop_local_stack = pop_local_stack;
  proc_ctx -> local_stack = local_stack;
  proc_ctx -> push_definitions = push_definitions;
  proc_ctx -> pop_definitions = pop_definitions;
  proc_ctx -> definitions = definitions;
  proc_ctx -> env = env;
  proc_ctx -> aislestore = aislestore;
  proc_ctx -> pool_list = pool_list;
  proc_ctx -> pool_string = pool_string;
  proc_ctx -> global_env = global_env;

  return proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public =
{
  create
};
