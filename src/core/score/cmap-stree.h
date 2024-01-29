#ifndef __CMAP_STREE_H__
#define __CMAP_STREE_H__

#include "cmap-stree-define.h"

typedef struct
{
  void * gt, * eq, * lt, * parent;

  char depth;
  int size;
} CMAP_STREE_NODE;

typedef struct CMAP_STREE_RUNNER CMAP_STREE_RUNNER;

struct CMAP_STREE_RUNNER
{
  CMAP_STREE_NODE * (*node)(void * node);
  int (*eval)(void * node, void * data);
  const char * (*log)(void * node);
  char gt_usable, lt_usable;
};

typedef struct CMAP_STREE_APPLY CMAP_STREE_APPLY;
typedef void (*CMAP_STREE_APPLY_FN)(void * node, char is_eq, void * data);

struct CMAP_STREE_APPLY
{
  CMAP_STREE_APPLY_FN before, between, after;
};

typedef struct
{
  void * (*find)(CMAP_STREE_RUNNER * runner, void * stree, void * data);

  void (*add)(CMAP_STREE_RUNNER * runner, void ** stree, void * node,
    void * data);
  void (*rm)(CMAP_STREE_RUNNER * runner, void ** stree, void * node);

  void (*apply)(CMAP_STREE_RUNNER * runner, void * stree,
    CMAP_STREE_APPLY * apply, char gt_first, char eq_apply, void * data);
  void (*quick_apply)(CMAP_STREE_RUNNER * runner, void * stree,
    CMAP_STREE_APPLY_FN apply, void * data);

  void (*log)(char lvl, CMAP_STREE_RUNNER * runner, void * stree);
} CMAP_STREE_PUBLIC;

CMAP_STREE_NODE * cmap_stree_node(void * node);

extern const CMAP_STREE_PUBLIC cmap_stree_public;

#endif
