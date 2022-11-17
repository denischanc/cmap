#ifndef __CMAP_LIST_H__
#define __CMAP_LIST_H__

#include <cmap/core.h>
#include "cmap-map.h"
#include "cmap-list-define.h"
#include <cmap/nature.h>

struct CMAP_LIST_s
{
  CMAP_MAP super;

  void * internal;

  int (*size)(CMAP_LIST * this);

  CMAP_LIST * (*set)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_LIST * this, int i);

  CMAP_LIST * (*add)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*rm)(CMAP_LIST * this, int i);

  CMAP_LIST * (*push)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*pop)(CMAP_LIST * this);

  CMAP_LIST * (*shift)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*unshift)(CMAP_LIST * this);

  void (*clear)(CMAP_LIST * this);
};

typedef struct
{
  CMAP_LIST * (*create)(int size_inc, const char * aisle);
  void (*init)(CMAP_LIST * list, int size_inc);
  CMAP_MAP * (*delete)(CMAP_LIST * list);

  int (*size)(CMAP_LIST * this);

  CMAP_LIST * (*set)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_LIST * this, int i);

  CMAP_LIST * (*add)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*rm)(CMAP_LIST * this, int i);

  CMAP_LIST * (*push)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*pop)(CMAP_LIST * this);

  CMAP_LIST * (*shift)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*unshift)(CMAP_LIST * this);

  void (*clear)(CMAP_LIST * this);
} CMAP_LIST_PUBLIC;

extern const CMAP_LIST_PUBLIC cmap_list_public;

#endif
