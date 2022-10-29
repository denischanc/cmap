#ifndef __CMAP_STRING_H__
#define __CMAP_STRING_H__

#include <cmap/core.h>
#include "cmap-map.h"

struct CMAP_STRING_s
{
  CMAP_MAP super;

  void * internal;

  const char * (*val)(CMAP_STRING * this);

  void (*append)(CMAP_STRING * this, const char * val);
  void (*append_sub)(CMAP_STRING * this, const char * val, int off_start,
    int off_stop);

  void (*clear)(CMAP_STRING * this);
};

typedef struct
{
  const char * nature;

  CMAP_STRING * (*create)(const char * val, int size_inc, const char * aisle);
  void (*init)(CMAP_STRING * string, const char * val, int size_inc);
  CMAP_MAP * (*delete)(CMAP_STRING * string);

  const char * (*val)(CMAP_STRING * this);

  void (*append)(CMAP_STRING * this, const char * val);
  void (*append_sub)(CMAP_STRING * this, const char * val, int off_start,
    int off_stop);

  void (*clear)(CMAP_STRING * this);
} CMAP_STRING_PUBLIC;

extern const CMAP_STRING_PUBLIC cmap_string_public;

#endif
