
#include "cmap-double.h"

#include "cmap-mem.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_DOUBLE_NATURE = "double";

/*******************************************************************************
*******************************************************************************/

double cmap_double_get(CMAP_DOUBLE * d)
{
  return d -> internal.val;
}

/*******************************************************************************
*******************************************************************************/

CMAP_DOUBLE * cmap_double_set(CMAP_DOUBLE * d, double val)
{
  d -> internal.val = val;
  return d;
}

/*******************************************************************************
*******************************************************************************/

CMAP_DOUBLE * cmap_double_init(CMAP_DOUBLE * d, CMAP_INITARGS * initargs,
  double val)
{
  cmap_map_init((CMAP_MAP *)d, initargs);

  d -> internal.val = val;

  return d;
}

CMAP_DOUBLE * cmap_double_create(double val, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  initargs.nature = CMAP_DOUBLE_NATURE;
  initargs.prototype = cmap_prototypestore_double(ps, proc_ctx);
  initargs.proc_ctx = proc_ctx;

  CMAP_MEM_ALLOC_PTR(d, CMAP_DOUBLE);
  return cmap_double_init(d, &initargs, val);
}
