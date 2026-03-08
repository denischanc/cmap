#ifndef __CMAP_PTR_INT_H__
#define __CMAP_PTR_INT_H__

typedef struct
{
  void * ptr;
  char allocated_by_this;

  CMAP_PTR_DELETE delete_ptr;
} CMAP_PTR_INTERNAL;

#endif
