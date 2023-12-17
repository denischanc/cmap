#ifndef __CMAP_STACK_DEFINE_H__
#define __CMAP_STACK_DEFINE_H__

#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_STACK_STRUCT_DECL(name, type) \
typedef struct CMAP_STACK_##name CMAP_STACK_##name; \
 \
struct CMAP_STACK_##name \
{ \
  CMAP_STACK_##name * next; \
  int size; \
 \
  type v; \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_STACK_DECL(name, type) \
CMAP_STACK_STRUCT_DECL(name, type) \
 \
typedef struct \
{ \
  void (*push)(CMAP_STACK_##name ** this, type v); \
  type (*pop)(CMAP_STACK_##name ** this); \
  void (*clean)(CMAP_STACK_##name ** this); \
} CMAP_STACK_##name##_PUBLIC; \
 \
extern const CMAP_STACK_##name##_PUBLIC cmap_stack_##name##_public;

/*******************************************************************************
*******************************************************************************/

#define CMAP_STACK_STATIC_FN_IMPL(name, type) \
static void stack_##name##_push(CMAP_STACK_##name ** this, type v) \
{ \
  CMAP_KERNEL_ALLOC_PTR(tmp, CMAP_STACK_##name); \
  tmp -> v = v; \
  tmp -> next = *this; \
  tmp -> size = (*this == NULL) ? 1 : (*this) -> size + 1; \
  *this = tmp; \
} \
 \
static type stack_##name##_pop(CMAP_STACK_##name ** this) \
{ \
  CMAP_STACK_##name * tmp = *this; \
  *this = tmp -> next; \
 \
  type ret = tmp -> v; \
  CMAP_KERNEL_FREE(tmp); \
  return ret; \
} \
 \
static void stack_##name##_clean(CMAP_STACK_##name ** this) \
{ \
  while(*this != NULL) \
  { \
    CMAP_STACK_##name * tmp = *this; \
    *this = tmp -> next; \
    CMAP_KERNEL_FREE(tmp); \
  } \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_STACK_IMPL(name, type) \
CMAP_STACK_STATIC_FN_IMPL(name, type) \
 \
const CMAP_STACK_##name##_PUBLIC cmap_stack_##name##_public = \
{ \
  stack_##name##_push, \
  stack_##name##_pop, \
  stack_##name##_clean \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_STACK_STATIC(name, type) \
CMAP_STACK_STRUCT_DECL(name, type) \
CMAP_STACK_STATIC_FN_IMPL(name, type)

/*******************************************************************************
*******************************************************************************/

#endif
