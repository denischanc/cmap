
#include "cmap-string.h"

#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * val;
  int size, size_inc, size_max;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_STRING_NATURE = "string";

/*******************************************************************************
*******************************************************************************/

static char * val(CMAP_STRING * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> val;
}

/*******************************************************************************
*******************************************************************************/

static inline int adjusted_size_max(INTERNAL * internal, int size)
{
  int size_inc = internal -> size_inc, new_size = (internal -> size + size),
    remainder = (new_size % size_inc);
  if(remainder == 0) return new_size;
  else return (new_size + size_inc - remainder);
}

/*******************************************************************************
*******************************************************************************/

static void append_(INTERNAL * internal, const char * val, int size_append)
{
  int off = internal -> size - 1,
    new_size_max = adjusted_size_max(internal, size_append);

  if(new_size_max > internal -> size_max)
  {
    CMAP_MEM * mem = CMAP_KERNEL_MEM;

    char * new_val = (char *)mem -> alloc(new_size_max),
      * old_val = internal -> val;
    memcpy(new_val, old_val, off);
    mem -> free(old_val);

    internal -> val = new_val;
    internal -> size_max = new_size_max;
  }

  int new_size = (internal -> size + size_append);
  char * int_val = internal -> val;
  memcpy(int_val + off, val, size_append);
  int_val[new_size - 1] = 0;

  internal -> size = new_size;
}

/*******************************************************************************
*******************************************************************************/

static void append(CMAP_STRING * this, const char * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  append_(internal, val, strlen(val));
}

/*******************************************************************************
*******************************************************************************/

static void append_sub(CMAP_STRING * this, const char * val,
  int off_start, int off_stop)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  append_(internal, val + off_start, off_stop - off_start);
}

/*******************************************************************************
*******************************************************************************/

static void clean(CMAP_STRING * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  internal -> val[0] = 0;
  internal -> size = 1;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = (INTERNAL *)((CMAP_STRING *)this) -> internal;
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_MEM_FREE(internal -> val, mem);
  CMAP_MEM_FREE(internal, mem);

  cmap_map_public.delete(this);
}

static CMAP_STRING * init(CMAP_STRING * this, CMAP_INITARGS * initargs,
  const char * val_, int size_inc)
{
  cmap_map_public.init((CMAP_MAP *)this, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;

  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  CMAP_MEM * mem = kernel -> mem();
  CMAP_KERNEL_CFG * cfg = kernel -> cfg();
  CMAP_MEM_ALLOC_PTR(internal, INTERNAL, mem);
  if(size_inc < cfg -> core.string_size_inc_min)
    size_inc = cfg -> core.string_size_inc;
  internal -> size_inc = size_inc;
  internal -> size = (strlen(val_) + 1);
  internal -> size_max = adjusted_size_max(internal, 0);
  internal -> val = (char *)mem -> alloc(internal -> size_max);
  memcpy(internal -> val, val_, internal -> size);

  this -> internal = internal;
  this -> val = val;
  this -> append = append;
  this -> append_sub = append_sub;
  this -> clean = clean;

  cmap_log_public.debug("[%p][%s] => [%.60s]", this, CMAP_STRING_NATURE, val_);

  return this;
}

static CMAP_STRING * create(const char * val, int size_inc,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  initargs.nature = CMAP_STRING_NATURE;
  initargs.prototype = CMAP_CALL_ARGS(ps, string_, proc_ctx);
  initargs.allocator = NULL;
  initargs.proc_ctx = proc_ctx;

  CMAP_STRING * this =
    (CMAP_STRING *)CMAP_KERNEL_MEM -> alloc(sizeof(CMAP_STRING));
  return init(this, &initargs, val, size_inc);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STRING_PUBLIC cmap_string_public =
{
  create, init, delete,
  val,
  append, append_sub,
  clean
};
