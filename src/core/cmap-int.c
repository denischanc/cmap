
#include "cmap-int.h"

#include "cmap-mem.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_INT_NATURE = "int";

/*******************************************************************************
*******************************************************************************/

int64_t cmap_int_get(CMAP_INT * i)
{
  return i -> internal.val;
}

/*******************************************************************************
*******************************************************************************/

CMAP_INT * cmap_int_set(CMAP_INT * i, int64_t val)
{
  i -> internal.val = val;
  return i;
}

/*******************************************************************************
*******************************************************************************/

CMAP_INT * cmap_int_init(CMAP_INT * i, CMAP_INITARGS * initargs, int64_t val)
{
  cmap_map_init((CMAP_MAP *)i, initargs);

  i -> internal.val = val;

  return i;
}

CMAP_INT * cmap_int_create(int64_t val, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  initargs.nature = CMAP_INT_NATURE;
  initargs.prototype = cmap_prototypestore_int(ps, proc_ctx);
  initargs.proc_ctx = proc_ctx;

  CMAP_MEM_ALLOC_PTR(i, CMAP_INT);
  return cmap_int_init(i, &initargs, val);
}
