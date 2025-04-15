#ifndef __CMAP_STACK_DEFINE_H__
#define __CMAP_STACK_DEFINE_H__

#define CMAP_STACK_TYPE(name) \
typedef struct CMAP_STACK_##name CMAP_STACK_##name;

#define CMAP_STACK_IMPL(name, type) \
struct CMAP_STACK_##name \
{ \
  type v; \
  CMAP_STACK_##name * next; \
}; \
 \
static void cmap_stack_##name##_push(CMAP_STACK_##name ** stack, type v) \
{ \
  CMAP_STACK_##name * tmp = \
    (CMAP_STACK_##name *)malloc(sizeof(CMAP_STACK_##name)); \
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
  free(tmp); \
  return ret; \
}

#define CMAP_STACK_DEF(name, type) \
CMAP_STACK_TYPE(name) \
CMAP_STACK_IMPL(name, type)

#endif
