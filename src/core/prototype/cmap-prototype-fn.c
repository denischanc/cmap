
#include "cmap-prototype-fn.h"

#include "cmap-prototype-util.h"

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_fn_require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_fn_init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
}
