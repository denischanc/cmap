
#include "cmap-fn.h"

#include <stdlib.h>
#include <string.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-util.h"
#include "cmap-proc-ctx.h"
#include "cmap-prototypestore.h"
#include "cmap-list.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_FN_NATURE = "fn";

const char * CMAP_PROTOTYPE_NAME = "prototype";

/*******************************************************************************
*******************************************************************************/

void cmap_fn_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list)
{
  CMAP_MAP ** definitions = &((CMAP_FN *)lc) -> internal.definitions;
  if(*definitions != NULL)
    cmap_slist_lc_ptr_push(list, (CMAP_LIFECYCLE **)definitions);

  cmap_map_nested(lc, list);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fn_require_definitions(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx)
{
  if(fn -> internal.definitions == NULL)
  {
    fn -> internal.definitions = cmap_map_create_root(proc_ctx);
    CMAP_INC_REFS(fn -> internal.definitions);
  }
  return fn -> internal.definitions;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fn_process(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_PROC_CTX * new_proc_ctx = cmap_proc_ctx_new(proc_ctx);

  CMAP_MAP * definitions = cmap_proc_ctx_local_definitions(new_proc_ctx);

  cmap_util_copy(definitions, fn -> internal.definitions);

  return cmap_proc_ctx_delete(new_proc_ctx,
    fn -> internal.process(new_proc_ctx, map, args));
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fn_new(CMAP_FN * fn, CMAP_LIST * args,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map = NULL;

  CMAP_MAP * prototype = CMAP_GET(fn, CMAP_PROTOTYPE_NAME);
  if(prototype != NULL) map = CMAP_PROTOTYPE_NEW(prototype, proc_ctx);
  else map = CMAP_MAP(proc_ctx);

  CMAP_FN_PROC(fn, proc_ctx, map, args);

  return map;
}

/*******************************************************************************
*******************************************************************************/

void cmap_fn_delete(CMAP_LIFECYCLE * lc)
{
  CMAP_MAP * definitions = ((CMAP_FN *)lc) -> internal.definitions;
  if(definitions != NULL) CMAP_DEC_REFS(definitions);

  cmap_map_delete(lc);
}

CMAP_FN * cmap_fn_init(CMAP_FN * fn, CMAP_INITARGS * initargs,
  CMAP_FN_TPL process)
{
  cmap_map_init((CMAP_MAP *)fn, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)fn;
  lc -> delete = cmap_fn_delete;
  lc -> nested = cmap_fn_nested;

  fn -> internal.process = process;
  fn -> internal.definitions = NULL;

  return fn;
}

CMAP_FN * cmap_fn_create(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  initargs.nature = CMAP_FN_NATURE;
  initargs.prototype = cmap_prototypestore_fn(ps, proc_ctx);
  initargs.proc_ctx = proc_ctx;

  return cmap_fn_init(CMAP_MEM_ALLOC(CMAP_FN), &initargs, process);
}
