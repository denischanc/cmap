#ifndef __CMAP_LIST_DEFINE_H__
#define __CMAP_LIST_DEFINE_H__

#define CMAP_LIST(size_inc, proc_ctx) cmap_list_create(size_inc, proc_ctx)

#define CMAP_LIST_SET(list, i, val, proc_ctx) \
  cmap_list_set(list, i, (CMAP_MAP *)val, proc_ctx)
#define CMAP_LIST_GET(list, i) cmap_list_get(list, i)

#define CMAP_LIST_ADD(list, i, val) cmap_list_add(list, i, (CMAP_MAP *)val)
#define CMAP_LIST_RM(list, i, proc_ctx) cmap_list_rm(list, i, proc_ctx)

#define CMAP_LIST_PUSH(list, val) cmap_list_push(list, (CMAP_MAP *)val)
#define CMAP_LIST_POP(list, proc_ctx) cmap_list_pop(list, proc_ctx)

#define CMAP_LIST_UNSHIFT(list, val) cmap_list_unshift(list, (CMAP_MAP *)val)
#define CMAP_LIST_SHIFT(list, proc_ctx) cmap_list_shift(list, proc_ctx)

#endif
