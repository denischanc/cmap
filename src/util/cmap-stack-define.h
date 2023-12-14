#ifndef __CMAP_STACK_DEFINE_H__
#define __CMAP_STACK_DEFINE_H__

#include "cmap-kernel-define.h"

#define CMAP_STACK_DEF(name, type) \
typedef struct CMAP_STACK_##name CMAP_STACK_##name; \
 \
struct CMAP_STACK_##name \
{ \
  type v; \
  CMAP_STACK_##name * next; \
}; \
 \
static void cmap_stack_##name##_push(CMAP_STACK_##name ** stack, type v) \
{ \
  CMAP_KERNEL_ALLOC_PTR(tmp, CMAP_STACK_##name); \
  tmp -> v = v; \
  tmp -> next = *stack; \
  *stack = tmp; \
} \
 \
static type cmap_stack_##name##_pop(CMAP_STACK_##name ** stack) \
{ \
  CMAP_STACK_##name * tmp = *stack; \
  *stack = (*stack) -> next; \
 \
  type ret = tmp -> v; \
  CMAP_KERNEL_FREE(tmp); \
  return ret; \
}

#endif
