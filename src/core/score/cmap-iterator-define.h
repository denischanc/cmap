#ifndef __CMAP_ITERATOR_DEFINE_H__
#define __CMAP_ITERATOR_DEFINE_H__

#define CMAP_ITERATOR_DECL(NAME, type) \
typedef struct CMAP_ITERATOR_##NAME CMAP_ITERATOR_##NAME; \
 \
struct CMAP_ITERATOR_##NAME \
{ \
  void (*delete)(CMAP_ITERATOR_##NAME * this); \
 \
  char (*has_next)(CMAP_ITERATOR_##NAME * this); \
  type (*next)(CMAP_ITERATOR_##NAME * this); \
  type (*get)(CMAP_ITERATOR_##NAME * this); \
  void (*rm)(CMAP_ITERATOR_##NAME * this); \
};

#endif
