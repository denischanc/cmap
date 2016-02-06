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

  int (*eval)(CMAP_TREE_RUNNER * this, void * node);
  char (*lt_usable)(CMAP_TREE_RUNNER * this);
  char (*gt_usable)(CMAP_TREE_RUNNER * this);
};

void * cmap_tree_find(CMAP_TREE_RUNNER * runner, void * tree);

void cmap_tree_add(CMAP_TREE_RUNNER * runner, void ** tree, void * node);
void cmap_tree_rm(CMAP_TREE_RUNNER * runner, void ** tree, void * node);

char cmap_tree_usable_true(CMAP_TREE_RUNNER * this);
char cmap_tree_usable_false(CMAP_TREE_RUNNER * this);

#endif
