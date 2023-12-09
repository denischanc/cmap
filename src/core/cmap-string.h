#ifndef __CMAP_STRING_H__
#define __CMAP_STRING_H__

#include "cmap-string-type.h"
#include "cmap-string-define.h"
#include "cmap-string-ext.h"
#include "cmap-map.h"
#include "cmap-lifecycle.h"

struct CMAP_STRING
{
  CMAP_MAP super;

  void * internal;

  char * (*val)(CMAP_STRING * this);

  void (*append)(CMAP_STRING * this, const char * val);
  void (*append_sub)(CMAP_STRING * this, const char * val, int off_start,
    int off_stop);

  void (*clear)(CMAP_STRING * this);
};

typedef struct
{
  CMAP_STRING * (*create)(const char * val, int size_inc,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);
  void (*init)(CMAP_STRING * string, const char * val, int size_inc);
  CMAP_LIFECYCLE * (*delete)(CMAP_STRING * string);

  char * (*val)(CMAP_STRING * this);

  void (*append)(CMAP_STRING * this, const char * val);
  void (*append_sub)(CMAP_STRING * this, const char * val, int off_start,
    int off_stop);

  void (*clear)(CMAP_STRING * this);
} CMAP_STRING_PUBLIC;

extern const CMAP_STRING_PUBLIC cmap_string_public;

#endif
