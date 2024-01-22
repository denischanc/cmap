#ifndef __CMAP_SLIST_DEFINE_H__
#define __CMAP_SLIST_DEFINE_H__

#include "cmap-lifecycle-type.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

#define CMAP_SLIST_LOOP(macro) \
  macro(LC, lc, CMAP_LIFECYCLE *, NULL) \
  macro(LC_PTR, lc_ptr, CMAP_LIFECYCLE **, NULL) \
  macro(MAP, map, CMAP_MAP *, NULL) \
  macro(PROC_CTX, proc_ctx, CMAP_PROC_CTX *, NULL) \
  macro(CHAR_PTR, char_ptr, char *, NULL)

#define CMAP_SLIST_DECL(NAME, name, type, dft) \
typedef struct CMAP_SLIST_##NAME CMAP_SLIST_##NAME; \
 \
typedef void (*CMAP_SLIST_##NAME##_APPLY_FN)(type * v, void * data); \
 \
struct CMAP_SLIST_##NAME \
{ \
  void (*delete)(CMAP_SLIST_##NAME * this); \
 \
  void (*push)(CMAP_SLIST_##NAME * this, type v); \
  type (*pop)(CMAP_SLIST_##NAME * this); \
 \
  void (*unshift)(CMAP_SLIST_##NAME * this, type v); \
  type (*shift)(CMAP_SLIST_##NAME * this); \
 \
  char (*add)(CMAP_SLIST_##NAME * this, int i, type v); \
  type (*rm)(CMAP_SLIST_##NAME * this, int i); \
 \
  type * (*get)(CMAP_SLIST_##NAME * this, int i); \
  type * (*first)(CMAP_SLIST_##NAME * this); \
  type * (*last)(CMAP_SLIST_##NAME * this); \
 \
  int (*size)(CMAP_SLIST_##NAME * this); \
 \
  void (*clean)(CMAP_SLIST_##NAME * this); \
 \
  void (*apply)(CMAP_SLIST_##NAME * this, CMAP_SLIST_##NAME##_APPLY_FN fn, \
    void * data); \
}; \
 \
typedef struct \
{ \
  CMAP_SLIST_##NAME * (*create)(int chunk_size); \
} CMAP_SLIST_##NAME##_PUBLIC; \
 \
extern const CMAP_SLIST_##NAME##_PUBLIC cmap_slist_##name##_public;

#endif
