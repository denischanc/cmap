#ifndef __CMAP_STACK_DEFINE_H__
#define __CMAP_STACK_DEFINE_H__

#define CMAP_STACK_DECL(NAME, name, type) \
typedef struct CMAP_STACK_##NAME CMAP_STACK_##NAME; \
 \
typedef void (*CMAP_STACK_##NAME##_APPLY_FN)(type v, void * data); \
 \
struct CMAP_STACK_##NAME \
{ \
  void (*delete)(CMAP_STACK_##NAME * this); \
 \
  void (*push)(CMAP_STACK_##NAME * this, type v); \
  type (*pop)(CMAP_STACK_##NAME * this); \
 \
  void (*unshift)(CMAP_STACK_##NAME * this, type v); \
  type (*shift)(CMAP_STACK_##NAME * this); \
 \
  void (*add)(CMAP_STACK_##NAME * this, int i, type v); \
  type (*rm)(CMAP_STACK_##NAME * this, int i); \
 \
  type (*get)(CMAP_STACK_##NAME * this, int i); \
  void (*set)(CMAP_STACK_##NAME * this, int i, type v); \
 \
  type (*first)(CMAP_STACK_##NAME * this); \
  type (*last)(CMAP_STACK_##NAME * this); \
 \
  int (*size)(CMAP_STACK_##NAME * this); \
 \
  void (*clean)(CMAP_STACK_##NAME * this); \
 \
  void (*apply)(CMAP_STACK_##NAME * this, CMAP_STACK_##NAME##_APPLY_FN fn, \
    void * data); \
}; \
 \
typedef struct \
{ \
  CMAP_STACK_##NAME * (*create)(int chunk_size); \
} CMAP_STACK_##NAME##_PUBLIC; \
 \
extern const CMAP_STACK_##NAME##_PUBLIC cmap_stack_##name##_public;

#endif
