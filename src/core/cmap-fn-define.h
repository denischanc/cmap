#ifndef __CMAP_FN_DEFINE_H__
#define __CMAP_FN_DEFINE_H__

#define CMAP_FN(process, proc_ctx) cmap_fn_create(process, proc_ctx)

#define CMAP_FN_PROC(fn, proc_ctx, map, args) \
  cmap_fn_process(fn, proc_ctx, (CMAP_MAP *)map, args)

#endif
