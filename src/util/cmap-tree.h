#ifndef __CMAP_TREE_H__
#define __CMAP_TREE_H__

#include "cmap-tree-define.h"

typedef struct CMAP_TREE_RUNNER_s CMAP_TREE_RUNNER;

struct CMAP_TREE_RUNNER_s
{
  void * internal_;

  void ** (*ge)(CMAP_TREE_RUNNER * this, void * node);
  void ** (*lt)(CMAP_TREE_RUNNER * this, void * node);
  void ** (*parent)(CMAP_TREE_RUNNER * this, void * node);

  int (*eval)(CMAP_TREE_RUNNER * this, void * node, void * data);
  char (*lt_usable)(CMAP_TREE_RUNNER * this);
  char (*gt_usable)(CMAP_TREE_RUNNER * this);
};

void * cmap_tree_find(CMAP_TREE_RUNNER * runner, void * tree, void * data);

void cmap_tree_add(CMAP_TREE_RUNNER * runner, void ** tree, void * node,
  void * data);
void cmap_tree_rm(CMAP_TREE_RUNNER * runner, void ** tree, void * node);

char cmap_tree_usable_true(CMAP_TREE_RUNNER * this);
char cmap_tree_usable_false(CMAP_TREE_RUNNER * this);

typedef struct CMAP_TREE_APPLY_s CMAP_TREE_APPLY;
typedef void (*CMAP_TREE_APPLY_FN)(CMAP_TREE_APPLY * this, void ** node,
  void * data);

struct CMAP_TREE_APPLY_s
{
  void * internal_;

  CMAP_TREE_APPLY_FN before, between, after;
};

void cmap_tree_apply(CMAP_TREE_RUNNER * runner, void ** tree,
  CMAP_TREE_APPLY * apply, char ge_first, void * data);

#endif
