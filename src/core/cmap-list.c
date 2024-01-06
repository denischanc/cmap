
#include "cmap-list.h"

#include <stdlib.h>
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_LIST_NATURE = "list";

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_LIFECYCLE * this)
{
  return CMAP_LIST_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static void nested_apply(CMAP_MAP ** val, void * data)
{
  CMAP_SLIST_LC_PTR * list = (CMAP_SLIST_LC_PTR *)data;
  CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)val);
}

static void nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list)
{
  CMAP_LIST * this_ = (CMAP_LIST *)this;
  CMAP_SLIST_MAP * this_list = (CMAP_SLIST_MAP *)this_ -> internal;
  CMAP_CALL_ARGS(this_list, apply, nested_apply, list);

  cmap_map_public.nested(this, list);
}

/*******************************************************************************
*******************************************************************************/

static int size(CMAP_LIST * this)
{
  return CMAP_CALL((CMAP_SLIST_MAP *)this -> internal, size);
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

    if(val != old_val)
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
    (val != NULL)) CMAP_INC_REFS(val);
  return this;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * rm(CMAP_LIST * this, int i)
{
  CMAP_MAP * ret = CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, rm, i);
  if(ret != NULL) CMAP_DEC_REFS(ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * push(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, push, val);
  if(val != NULL) CMAP_INC_REFS(val);
  return this;
}

static CMAP_MAP * pop(CMAP_LIST * this)
{
  CMAP_MAP * ret = CMAP_CALL((CMAP_SLIST_MAP *)this -> internal, pop);
  if(ret != NULL) CMAP_DEC_REFS(ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * unshift(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS((CMAP_SLIST_MAP *)this -> internal, unshift, val);
  if(val != NULL) CMAP_INC_REFS(val);
  return this;
}

static CMAP_MAP * shift(CMAP_LIST * this)
{
  CMAP_MAP * ret = CMAP_CALL((CMAP_SLIST_MAP *)this -> internal, shift);
  if(ret != NULL) CMAP_DEC_REFS(ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST_VAL_FN fn;
  void * data;
} APPLY_APPLY_DATA;

static void apply_apply(CMAP_MAP ** val, void * data)
{
  APPLY_APPLY_DATA * data_ = (APPLY_APPLY_DATA *)data;

  CMAP_MAP * old_val = *val;
  data_ -> fn(val, data_ -> data);

  CMAP_MAP * new_val = *val;
  if(new_val != old_val)
  {
    if(old_val != NULL) CMAP_DEC_REFS(old_val);
    if(new_val != NULL) CMAP_INC_REFS(new_val);
  }
}

static void apply(CMAP_LIST * this, CMAP_LIST_VAL_FN fn, void * data)
{
  APPLY_APPLY_DATA data_ = { fn, data };
  CMAP_SLIST_MAP * this_list = (CMAP_SLIST_MAP *)this -> internal;
  CMAP_CALL_ARGS(this_list, apply, apply_apply, &data_);
}

/*******************************************************************************
*******************************************************************************/

static void clean_apply(CMAP_MAP ** val, void * data)
{
  if(*val != NULL) CMAP_DEC_REFS(*val);
}

static void clean(CMAP_LIST * this)
{
  CMAP_SLIST_MAP * this_list = (CMAP_SLIST_MAP *)this -> internal;
  CMAP_CALL_ARGS(this_list, apply, clean_apply, NULL);
  CMAP_CALL(this_list, clean);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  CMAP_CALL((CMAP_SLIST_MAP *)((CMAP_LIST *)this) -> internal, delete);

  cmap_map_public.delete(this);
}

static void init(CMAP_LIST * this, int chunk_size)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;
  lc -> nature = nature;
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
}

static CMAP_LIST * create(int chunk_size, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_list = CMAP_CALL_ARGS(ps, list_, proc_ctx);
  CMAP_LIST * this = CMAP_PROTOTYPE_NEW(prototype_list, CMAP_LIST, proc_ctx);
  init(this, chunk_size);
  return this;
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
