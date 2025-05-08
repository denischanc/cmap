
#include "cmap-list.h"

#include <stdlib.h>
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-kernel.h"
#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_LIST_NATURE = "list";

/*******************************************************************************
*******************************************************************************/

static void nested_apply(CMAP_MAP ** val, void * data)
{
  if(*val != NULL)
  {
    CMAP_SLIST_LC_PTR * list = (CMAP_SLIST_LC_PTR *)data;
    CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)val);
  }
}

static void nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list)
{
  if(!CMAP_IS_GHOST(this))
  {
    CMAP_LIST * this_ = (CMAP_LIST *)this;
    CMAP_SLIST_MAP * this_list = this_ -> internal;
    CMAP_CALL_ARGS(this_list, apply, nested_apply, list);
  }

  cmap_map_public.nested(this, list);
}

/*******************************************************************************
*******************************************************************************/

static int size(CMAP_LIST * this)
{
  CMAP_SLIST_MAP * this_list = (CMAP_SLIST_MAP *)this -> internal;
  return CMAP_CALL(this_list, size);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * set(CMAP_LIST * this, int i, CMAP_MAP * val)
{
  CMAP_MAP ** val_ptr =
    CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, get, i);
  if(val_ptr != NULL)
  {
    CMAP_MAP * old_val = *val_ptr;
    *val_ptr = val;

    if(!CMAP_IS_GHOST(this) && (val != old_val))
    {
      if(val != NULL) CMAP_INC_REFS(val);
      if(old_val != NULL) CMAP_DEC_REFS(old_val);
    }
  }
  return this;
}

static CMAP_MAP * get(CMAP_LIST * this, int i)
{
  CMAP_MAP ** ret = CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, get, i);
  return (ret == NULL) ? NULL : *ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * add(CMAP_LIST * this, int i, CMAP_MAP * val)
{
  if(CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, add, i, val) &&
    !CMAP_IS_GHOST(this) && (val != NULL)) CMAP_INC_REFS(val);
  return this;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * rm(CMAP_LIST * this, int i)
{
  CMAP_MAP * ret = CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, rm, i);
  if(!CMAP_IS_GHOST(this) && (ret != NULL)) CMAP_DEC_REFS(ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * push(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, push, val);
  if(!CMAP_IS_GHOST(this) && (val != NULL)) CMAP_INC_REFS(val);
  return this;
}

static CMAP_MAP * pop(CMAP_LIST * this)
{
  CMAP_MAP * ret = CMAP_CALL((CMAP_SLIST_MAP *)this -> internal, pop);
  if(!CMAP_IS_GHOST(this) && (ret != NULL)) CMAP_DEC_REFS(ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * unshift(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, unshift, val);
  if(!CMAP_IS_GHOST(this) && (val != NULL)) CMAP_INC_REFS(val);
  return this;
}

static CMAP_MAP * shift(CMAP_LIST * this)
{
  CMAP_MAP * ret = CMAP_CALL((CMAP_SLIST_MAP *)this -> internal, shift);
  if(!CMAP_IS_GHOST(this) && (ret != NULL)) CMAP_DEC_REFS(ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST_VAL_FN fn;
  void * data;
  char ghost;
} APPLY_APPLY_DATA;

static void apply_apply(CMAP_MAP ** val, void * data)
{
  APPLY_APPLY_DATA * data_ = data;

  CMAP_MAP * old_val = *val;
  data_ -> fn(val, data_ -> data);

  if(!data_ -> ghost)
  {
    CMAP_MAP * new_val = *val;
    if(new_val != old_val)
    {
      if(old_val != NULL) CMAP_DEC_REFS(old_val);
      if(new_val != NULL) CMAP_INC_REFS(new_val);
    }
  }
}

static void apply(CMAP_LIST * this, CMAP_LIST_VAL_FN fn, void * data)
{
  APPLY_APPLY_DATA data_ = {fn, data, CMAP_IS_GHOST(this)};
  CMAP_SLIST_MAP * this_list = this -> internal;
  CMAP_APPLY(this_list, apply_apply, &data_);
}

/*******************************************************************************
*******************************************************************************/

static void clean_apply(CMAP_MAP ** val, void * data)
{
  if(*val != NULL) CMAP_DEC_REFS(*val);
}

static void clean(CMAP_LIST * this)
{
  CMAP_SLIST_MAP * this_list = this -> internal;
  if(!CMAP_IS_GHOST(this)) CMAP_CALL_ARGS(this_list, apply, clean_apply, NULL);
  CMAP_CALL(this_list, clean);
}

/*******************************************************************************
*******************************************************************************/

static void delete_apply(CMAP_MAP ** val, void * data)
{
  if(*val != NULL)
  {
    cmap_log_public.debug("[%p][%s]-nested->[[%p]==>refs--]",
      data, CMAP_NATURE(data), *val);
    CMAP_DEC_REFS(*val);
  }
}

static void delete(CMAP_LIFECYCLE * this)
{
  CMAP_SLIST_MAP * this_list = ((CMAP_LIST *)this) -> internal;
  if(!CMAP_IS_GHOST(this)) CMAP_CALL_ARGS(this_list, apply, delete_apply, this);
  CMAP_CALL(this_list, delete);

  cmap_map_public.delete(this);
}

static CMAP_LIST * init(CMAP_LIST * this, CMAP_INITARGS * initargs,
  int chunk_size)
{
  cmap_map_public.init((CMAP_MAP *)this, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;
  lc -> nested = nested;

  this -> internal = cmap_slist_map_public.create(chunk_size);
  this -> size = size;
  this -> set = set;
  this -> get = get;
  this -> add = add;
  this -> rm = rm;
  this -> push = push;
  this -> pop = pop;
  this -> unshift = unshift;
  this -> shift = shift;
  this -> apply = apply;
  this -> clean = clean;

  return this;
}

static CMAP_LIST * create(int chunk_size, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  initargs.nature = CMAP_LIST_NATURE;
  initargs.prototype = CMAP_CALL_ARGS(ps, list_, proc_ctx);
  initargs.allocator = NULL;
  initargs.proc_ctx = proc_ctx;

  CMAP_LIST * this = (CMAP_LIST *)CMAP_KERNEL_MEM -> alloc(sizeof(CMAP_LIST));
  return init(this, &initargs, chunk_size);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LIST_PUBLIC cmap_list_public =
{
  create, init, delete,
  nested,
  size,
  set, get,
  add,  rm,
  push, pop,
  unshift, shift,
  apply,
  clean
};
