#ifndef __CMAP_TREE_H__
#define __CMAP_TREE_H__

#include "cmap-tree-define.h"

typedef struct
{
  void * gt, * eq, * lt, * parent;

  char depth;
  int size;
} CMAP_TREE_NODE;

typedef struct CMAP_TREE_RUNNER CMAP_TREE_RUNNER;

struct CMAP_TREE_RUNNER
{
  CMAP_TREE_NODE * (*node)(void * node);
  int (*eval)(void * node, void * data);
  char gt_usable, lt_usable;
};

typedef struct CMAP_TREE_APPLY CMAP_TREE_APPLY;
typedef void (*CMAP_TREE_APPLY_FN)(void * node, void * data);

struct CMAP_TREE_APPLY
{
  CMAP_TREE_APPLY_FN before, between, after;
};

typedef struct
{
  void * (*find)(CMAP_TREE_RUNNER * runner, void * tree, void * data);

  void (*add)(CMAP_TREE_RUNNER * runner, void ** tree, void * node,
    void * data);
  void (*rm)(CMAP_TREE_RUNNER * runner, void ** tree, void * node);

  void (*apply)(CMAP_TREE_RUNNER * runner, void * tree,
    CMAP_TREE_APPLY * apply, char gt_first, void * data);

  void (*clean)(CMAP_TREE_RUNNER * runner, void ** tree,
    CMAP_TREE_APPLY_FN clean_node, void * data);

  void (*log)(char lvl, CMAP_TREE_RUNNER * runner, void * tree,
    void * (*ptr)(void * node));
} CMAP_TREE_PUBLIC;

CMAP_TREE_NODE * cmap_tree_node(void * node);

extern const CMAP_TREE_PUBLIC cmap_tree_public;

#endif
