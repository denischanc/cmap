
#include "cmap-list.h"

#include <stdlib.h>
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-mem.h"
#include "cmap-log.h"
#include "cmap-core.h"

/*******************************************************************************
*******************************************************************************/

static void nested_apply(CMAP_MAP ** val, void * data)
{
  if(*val != NULL)
  {
    CMAP_SLIST_LC_PTR * list = (CMAP_SLIST_LC_PTR *)data;
    cmap_slist_lc_ptr_push(list, (CMAP_LIFECYCLE **)val);
  }
}

void cmap_list_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx)
{
  if(!CMAP_IS_GHOST(lc))
  {
    CMAP_LIST * list_ = (CMAP_LIST *)lc;
    cmap_slist_map_apply(list_ -> internal.list, nested_apply, list);
  }

  cmap_map_nested(lc, list, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

int cmap_list_size(CMAP_LIST * list)
{
  return cmap_slist_map_size(list -> internal.list);
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP ** val_ptr = cmap_slist_map_get(list -> internal.list, i);
  if(val_ptr != NULL)
  {
    CMAP_MAP * old_val = *val_ptr;
    *val_ptr = val;

    if(!CMAP_IS_GHOST(list) && (val != old_val))
    {
      if(val != NULL) CMAP_INC_REFS(val);
      if(old_val != NULL) CMAP_DEC_REFS(old_val, proc_ctx);
    }
  }
  return list;
}

CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i)
{
  CMAP_MAP ** ret = cmap_slist_map_get(list -> internal.list, i);
  return (ret == NULL) ? NULL : *ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_list_add(CMAP_LIST * list, int i, CMAP_MAP * val)
{
  if(cmap_slist_map_add(list -> internal.list, i, val) &&
    !CMAP_IS_GHOST(list) && (val != NULL)) CMAP_INC_REFS(val);
  return list;
}

CMAP_MAP * cmap_list_rm(CMAP_LIST * list, int i, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * ret = cmap_slist_map_rm(list -> internal.list, i);
  if(!CMAP_IS_GHOST(list) && (ret != NULL)) CMAP_DEC_REFS(ret, proc_ctx);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_list_push(CMAP_LIST * list, CMAP_MAP * val)
{
  cmap_slist_map_push(list -> internal.list, val);
  if(!CMAP_IS_GHOST(list) && (val != NULL)) CMAP_INC_REFS(val);
  return list;
}

CMAP_MAP * cmap_list_pop(CMAP_LIST * list, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * ret = cmap_slist_map_pop(list -> internal.list);
  if(!CMAP_IS_GHOST(list) && (ret != NULL)) CMAP_DEC_REFS(ret, proc_ctx);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_list_unshift(CMAP_LIST * list, CMAP_MAP * val)
{
  cmap_slist_map_unshift(list -> internal.list, val);
  if(!CMAP_IS_GHOST(list) && (val != NULL)) CMAP_INC_REFS(val);
  return list;
}

CMAP_MAP * cmap_list_shift(CMAP_LIST * list, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * ret = cmap_slist_map_shift(list -> internal.list);
  if(!CMAP_IS_GHOST(list) && (ret != NULL)) CMAP_DEC_REFS(ret, proc_ctx);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST_VAL_FN fn;
  void * data;
  char ghost;
  CMAP_PROC_CTX * proc_ctx;
} APPLY_APPLY_DATA;

static void apply_apply(CMAP_MAP ** val, void * data)
{
  APPLY_APPLY_DATA * data_ = data;

  CMAP_MAP * old_val = *val;
  data_ -> fn(val, data_ -> data, data_ -> proc_ctx);

  if(!data_ -> ghost)
  {
    CMAP_MAP * new_val = *val;
    if(new_val != old_val)
    {
      if(old_val != NULL) CMAP_DEC_REFS(old_val, data_ -> proc_ctx);
      if(new_val != NULL) CMAP_INC_REFS(new_val);
    }
  }
}

void cmap_list_apply(CMAP_LIST * list, CMAP_LIST_VAL_FN fn, void * data,
  CMAP_PROC_CTX * proc_ctx)
{
  APPLY_APPLY_DATA data_ = {fn, data, CMAP_IS_GHOST(list), proc_ctx};
  cmap_slist_map_apply(list -> internal.list, apply_apply, &data_);
}

/*******************************************************************************
*******************************************************************************/

static void clean_apply(CMAP_MAP ** val, void * data)
{
  if(*val != NULL) CMAP_DEC_REFS(*val, data);
}

void cmap_list_clean(CMAP_LIST * list, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_SLIST_MAP * list_ = list -> internal.list;
  if(!CMAP_IS_GHOST(list)) cmap_slist_map_apply(list_, clean_apply, proc_ctx);
  cmap_slist_map_clean(list_);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;
  CMAP_PROC_CTX * proc_ctx;
} DELETE_APPLY_DATA;

static void delete_apply(CMAP_MAP ** val, void * data)
{
  if(*val != NULL)
  {
    DELETE_APPLY_DATA * data_ = data;
    CMAP_LIFECYCLE * lc = data_ -> lc;
    cmap_log_debug("[%p][%s]-nested->[[%p]==>refs--]",
      lc, CMAP_NATURE_CHAR(lc), *val);
    CMAP_DEC_REFS(*val, data_ -> proc_ctx);
  }
}

void cmap_list_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_SLIST_MAP * list = ((CMAP_LIST *)lc) -> internal.list;
  if(!CMAP_IS_GHOST(lc))
  {
    DELETE_APPLY_DATA data = {lc, proc_ctx};
    cmap_slist_map_apply(list, delete_apply, &data);
  }
  cmap_slist_map_delete(list);

  cmap_map_delete(lc, proc_ctx);
}

CMAP_LIST * cmap_list_init(CMAP_LIST * list, CMAP_INITARGS * initargs,
  int chunk_size)
{
  cmap_map_init((CMAP_MAP *)list, initargs);

  list -> internal.list = cmap_slist_map_create(chunk_size);

  return list;
}

CMAP_LIST * cmap_list_create(int chunk_size, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  initargs.nature = CMAP_LIST_NATURE;
  initargs.prototype = cmap_prototypestore_list(ps, proc_ctx);
  initargs.proc_ctx = proc_ctx;

  return cmap_list_init(CMAP_MEM_ALLOC(CMAP_LIST), &initargs, chunk_size);
}
