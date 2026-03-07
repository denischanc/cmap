#ifndef __CMAP_ITERATOR_DEFINE_H__
#define __CMAP_ITERATOR_DEFINE_H__

#define CMAP_ITERATOR_DECL(NAME, name, type) \
typedef struct CMAP_ITERATOR_##NAME CMAP_ITERATOR_##NAME; \
 \
char cmap_iterator_##name##_has_next(CMAP_ITERATOR_##NAME * it); \
type cmap_iterator_##name##_next(CMAP_ITERATOR_##NAME * it); \
type cmap_iterator_##name##_get(CMAP_ITERATOR_##NAME * it); \
void cmap_iterator_##name##_rm(CMAP_ITERATOR_##NAME * it); \
 \
void cmap_iterator_##name##_delete(CMAP_ITERATOR_##NAME * it);

#endif
