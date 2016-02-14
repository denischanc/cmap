
#include "cmap-kernel.h"
#include "cmap-tree.h"
#include "cmap-common.h"

#include <stdlib.h>
#include <stdio.h>

#define SIZE 10

typedef struct
{
  int nb_;
  CMAP_TREE_STRUCT;
} NB;

static int nb__eval(CMAP_TREE_RUNNER * runner, void * node)
{
  NB * nb = (NB *)runner -> internal_;
  return (((NB *)node) -> nb_ - nb -> nb_);
}

CMAP_TREE_RUNNER(NB, nb, false, false)

static void nb_printf(CMAP_TREE_APPLY * this, void * node)
{
  NB * nb = (NB *)node;
  printf("%d ", nb -> nb_);
}

int main(int argc, char * argv[])
{
  cmap_kernel_create(NULL);
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  NB * nb_tree = NULL, * tmp;

  int i;
  for(i = 0; i < SIZE; i++)
  {
    tmp = (NB *)mem -> alloc(sizeof(NB));
    tmp -> nb_ = (i + 1);

    nb_runner_.internal_ = tmp;
    cmap_tree_add(&nb_runner_, (void **)&nb_tree, tmp);
  }

  CMAP_TREE_APPLY apply;
  apply.before = NULL;
  apply.between = nb_printf;
  apply.after = NULL;
  cmap_tree_apply(&nb_runner_, nb_tree, &apply, CMAP_T);
  printf("\n");

  return EXIT_SUCCESS;
}
