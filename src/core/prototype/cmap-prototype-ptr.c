
#include "cmap-prototype-ptr.h"

#include "cmap-prototype-util.h"

/*******************************************************************************
*******************************************************************************/

static void require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_public.require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static void init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_PTR_PUBLIC cmap_prototype_ptr_public =
{
  require,
  init
};
