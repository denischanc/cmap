#ifndef __CMAP_STREE_H__
#define __CMAP_STREE_H__

#include "cmap-stree-define.h"
#include <stdint.h>

typedef struct CMAP_STREE_NODE CMAP_STREE_NODE;

struct CMAP_STREE_NODE
{
  CMAP_STREE_NODE * gt, * eq, * lt, * parent;

  char depth;
  int size;
};

typedef struct
{
  int64_t (*eval)(CMAP_STREE_NODE * node, void * data);
  const char * (*log)(CMAP_STREE_NODE * node);
  char gt_usable, lt_usable;
} CMAP_STREE_RUNNER;

typedef void (*CMAP_STREE_APPLY_FN)(CMAP_STREE_NODE * node, char is_eq,
  void * data);

typedef struct
{
  CMAP_STREE_APPLY_FN before, between, after;
} CMAP_STREE_APPLY;

typedef struct
{
  CMAP_STREE_NODE * (*find)(CMAP_STREE_RUNNER * runner, CMAP_STREE_NODE * stree,
    void * data);

  void (*add)(CMAP_STREE_RUNNER * runner, CMAP_STREE_NODE ** stree,
    CMAP_STREE_NODE * node, void * data);
  void (*rm)(CMAP_STREE_NODE ** stree, CMAP_STREE_NODE * node);

  void (*apply)(CMAP_STREE_NODE * stree, CMAP_STREE_APPLY * apply,
    char gt_first, char eq_apply, void * data);
  void (*quick_apply)(CMAP_STREE_NODE * stree, CMAP_STREE_APPLY_FN apply,
    void * data);

  void (*log)(char lvl, CMAP_STREE_RUNNER * runner, CMAP_STREE_NODE * stree);
} CMAP_STREE_PUBLIC;

extern const CMAP_STREE_PUBLIC cmap_stree_public;

#endif
