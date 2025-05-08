#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

#include "cmap-map-type.h"
#include "cmap-map-define.h"
#include "cmap-map-ext.h"
#include "cmap-lifecycle.h"

typedef void (*CMAP_MAP_ENTRY_FN)(const char * key, CMAP_MAP ** val,
  void * data);

struct CMAP_MAP
{
  CMAP_LIFECYCLE super;

  void * internal;

  CMAP_MAP * (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  CMAP_MAP * (*new)(CMAP_MAP * this, CMAP_PROC_CTX * proc_ctx);

  char (*is_key)(CMAP_MAP * this, const char * key);
  CMAP_LIST * (*keys)(CMAP_MAP * this, CMAP_PROC_CTX * proc_ctx);

  void (*apply)(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);

  void (*clean)(CMAP_MAP * this);

  void (*ghost)(CMAP_MAP * this);
  char (*is_ghost)(CMAP_MAP * this);
};

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx);
  CMAP_MAP * (*create_root)(CMAP_PROC_CTX * proc_ctx);
  CMAP_MAP * (*init)(CMAP_MAP * this, CMAP_INITARGS * initargs);
  void (*delete)(CMAP_LIFECYCLE * this);

  void (*nested)(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list);

  CMAP_MAP * (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  CMAP_MAP * (*new)(CMAP_MAP * this, CMAP_PROC_CTX * proc_ctx);

  char (*is_key)(CMAP_MAP * this, const char * key);
  CMAP_LIST * (*keys)(CMAP_MAP * this, CMAP_PROC_CTX * proc_ctx);

  void (*apply)(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);

  void (*clean)(CMAP_MAP * this);

  void (*ghost)(CMAP_MAP * this);
  char (*is_ghost)(CMAP_MAP * this);
} CMAP_MAP_PUBLIC;

extern const CMAP_MAP_PUBLIC cmap_map_public;

#endif
