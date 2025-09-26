#ifndef __CMAP_STACK_DEFINE_H__
#define __CMAP_STACK_DEFINE_H__

#define CMAP_STACK_TYPE(NAME) \
typedef struct CMAP_STACK_##NAME CMAP_STACK_##NAME;

#define CMAP_STACK_IMPL(NAME, name, type) \
struct CMAP_STACK_##NAME \
{ \
  type v; \
  CMAP_STACK_##NAME * next; \
}; \
 \
static void cmap_stack_##name##_push(CMAP_STACK_##NAME ** stack, type v) \
{ \
  CMAP_STACK_##NAME * tmp = \
    (CMAP_STACK_##NAME *)malloc(sizeof(CMAP_STACK_##NAME)); \
  tmp -> v = v; \
  tmp -> next = *stack; \
  *stack = tmp; \
} \
 \
static type cmap_stack_##name##_pop(CMAP_STACK_##NAME ** stack) \
{ \
  CMAP_STACK_##NAME * tmp = *stack; \
  *stack = (*stack) -> next; \
 \
  type ret = tmp -> v; \
  free(tmp); \
  return ret; \
} \
 \
__attribute__((unused)) static CMAP_STACK_##NAME * cmap_stack_##name##_stack( \
  type * v) \
{ \
  return (CMAP_STACK_##NAME *)v; \
} \
 \
__attribute__((unused)) static type * cmap_stack_##name##_v( \
  CMAP_STACK_##NAME * stack) \
{ \
  return (type *)stack; \
}

#define CMAP_STACK_DEF(NAME, name, type) \
CMAP_STACK_TYPE(NAME) \
CMAP_STACK_IMPL(NAME, name, type)

#endif
