#ifndef __CMAP_LIST_H__
#define __CMAP_LIST_H__

#include "cmap-map.h"

extern const char * CMAP_LIST_NATURE;

typedef struct CMAP_LIST_s CMAP_LIST;

struct CMAP_LIST_s
{
  CMAP_MAP super_;

  void * internal_;

  int (*size)(CMAP_LIST * this);

  void (*set)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_LIST * this, int i);

  void (*add)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*rm)(CMAP_LIST * this, int i);

  void (*push)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*pop)(CMAP_LIST * this);

  void (*shift)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*unshift)(CMAP_LIST * this);
};

int cmap_list__size(CMAP_LIST * this);

void cmap_list__set(CMAP_LIST * this, int i, CMAP_MAP * val);
CMAP_MAP * cmap_list__get(CMAP_LIST * this, int i);

void cmap_list__add(CMAP_LIST * this, int i, CMAP_MAP * val);
CMAP_MAP * cmap_list__rm(CMAP_LIST * this, int i);

void cmap_list__push(CMAP_LIST * this, CMAP_MAP * val);
CMAP_MAP * cmap_list__pop(CMAP_LIST * this);

void cmap_list__shift(CMAP_LIST * this, CMAP_MAP * val);
CMAP_MAP * cmap_list__unshift(CMAP_LIST * this);

CMAP_LIST * cmap_list_create(int size_inc);
void cmap_list_init(CMAP_LIST * list, int size_inc);
void cmap_list_delete(CMAP_LIST * list);

#endif
