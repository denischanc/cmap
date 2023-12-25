#ifndef __CMAP_TREE_H__
#define __CMAP_TREE_H__

#include "cmap-tree-define.h"

typedef struct
{
  CMAP_TREE_LOOP(CMAP_TREE_WAY_FIELD)
} CMAP_TREE_NODE;

typedef struct CMAP_TREE_RUNNER CMAP_TREE_RUNNER;

struct CMAP_TREE_RUNNER
{
  void * internal;

  CMAP_TREE_LOOP(CMAP_TREE_WAY_FN_PTR)

  int (*eval)(CMAP_TREE_RUNNER * this, void * node, void * data);
  char (*lt_usable)(CMAP_TREE_RUNNER * this);
  char (*gt_usable)(CMAP_TREE_RUNNER * this);
};

typedef struct CMAP_TREE_APPLY CMAP_TREE_APPLY;
typedef void (*CMAP_TREE_APPLY_FN)(CMAP_TREE_APPLY * this, void ** node,
  void * data);

struct CMAP_TREE_APPLY
{
  CMAP_TREE_APPLY_FN before, between, after;
};

typedef void (*CMAP_TREE_CLEAN_FN)(void * node, void * data);

typedef struct
{
  void * (*find)(CMAP_TREE_RUNNER * runner, void * tree, void * data);

  void (*add)(CMAP_TREE_RUNNER * runner, void ** tree, void * node,
    void * data);
  void (*rm)(CMAP_TREE_RUNNER * runner, void ** tree, void * node);

  void (*apply)(CMAP_TREE_RUNNER * runner, void ** tree,
    CMAP_TREE_APPLY * apply, char ge_first, void * data);

  void (*clean)(CMAP_TREE_RUNNER * runner, void ** tree,
    CMAP_TREE_CLEAN_FN clean_node, void * data);
} CMAP_TREE_PUBLIC;

char cmap_tree_usable_true(CMAP_TREE_RUNNER * this);
char cmap_tree_usable_false(CMAP_TREE_RUNNER * this);

CMAP_TREE_LOOP(CMAP_TREE_WAY_FN)

extern const CMAP_TREE_PUBLIC cmap_tree_public;

#endif
