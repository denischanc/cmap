#ifndef __CMAP_UTIL_TREE_H__
#define __CMAP_UTIL_TREE_H__

typedef struct CMAP_UTIL_TREE_HANDLER_s CMAP_UTIL_TREE_HANDLER;

struct CMAP_UTIL_TREE_HANDLER_s
{
  void * internal_;

  void ** (*ge)(CMAP_UTIL_TREE_HANDLER * this, void * node);
  void ** (*lt)(CMAP_UTIL_TREE_HANDLER * this, void * node);
  void ** (*parent)(CMAP_UTIL_TREE_HANDLER * this, void * node);

  int (*eval)(CMAP_UTIL_TREE_HANDLER * this, void * node);
  char (*lt_usable)(CMAP_UTIL_TREE_HANDLER * this);
  char (*gt_usable)(CMAP_UTIL_TREE_HANDLER * this);
};

void * cmap_util_tree_find(CMAP_UTIL_TREE_HANDLER * handler, void * tree);

void cmap_util_tree_add(CMAP_UTIL_TREE_HANDLER * handler, void ** tree,
  void * node);
void cmap_util_tree_rm(CMAP_UTIL_TREE_HANDLER * handler, void ** tree,
  void * node);

#endif
