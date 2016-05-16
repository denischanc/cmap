/*******************************************************************************
@TEST_DESC@ Check memory with tree
*******************************************************************************/

#include "cmap-mem.h"
#include "cmap-tree.h"
#include "cmap-common.h"

#include <stdlib.h>
#include <stdio.h>
#include "test-assert.h"

/*******************************************************************************
*******************************************************************************/

#define SIZE 10
#define MAX (1 << 16)
//#define DEBUG

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int nb_;
  CMAP_TREE_STRUCT;
} NB;

static int CMAP_TREE_EVALFN_NAME(nb)(CMAP_TREE_RUNNER * runner, void * node,
  void * data)
{
  NB * node1 = (NB *)node, * node2 = (NB *)data;
  return (node1 -> nb_ - node2 -> nb_);
}

CMAP_TREE_RUNNER(NB, nb, NULL, false, false)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int i_;
  int * list_;
} TREE2LIST_ARGS;

static void nb_tree2list(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  TREE2LIST_ARGS * args = (TREE2LIST_ARGS *)data;
  int nb = ((NB *)*node) -> nb_;
  args -> list_[args -> i_++] = nb;

#ifdef DEBUG
  printf("%d ", nb);
#endif
}

static void nb_delete(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  CMAP_MEM * mem = (CMAP_MEM *)data;
  mem -> free(*node);
  *node = NULL;
}

/*******************************************************************************
*******************************************************************************/

static char check_sort(char ge_first, TREE2LIST_ARGS * args,
  CMAP_TREE_APPLY * apply, NB ** tree)
{
  /********** Fill list */
  int i;
  for(i = 0; i < SIZE; i++)
  {
    args -> list_[i] = ge_first ? i : SIZE - i - 1;
  }

  args -> i_ = 0;
  CMAP_TREE_APPLYFN(nb, tree, *apply, ge_first, args);
#ifdef DEBUG
  printf("\n");
#endif

  /********** Check order */
  int prev = ge_first ? MAX : 0, cur;
  for(i = 0; i < SIZE; i++)
  {
    cur = args -> list_[i];
    if(ge_first)
    {
      if(prev < cur) return CMAP_F;
    }
    else
    {
      if(prev > cur) return CMAP_F;
    }
    prev = cur;
  }

  return CMAP_T;
}

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  CMAP_MEM * mem = cmap_mem_create(0);
  NB * nb_tree = NULL, * tmp;

  /********** Fill tree */
  int i;
  for(i = 0; i < SIZE; i++)
  {
    tmp = (NB *)mem -> alloc(sizeof(NB));
    tmp -> nb_ = (random() % MAX);

    CMAP_TREE_ADDFN(nb, &nb_tree, tmp, tmp);
  }

  /********** Check tree */
  TREE2LIST_ARGS args;
  args.list_ = (int *)mem -> alloc(SIZE * sizeof(int));

  CMAP_TREE_APPLY apply;
  CMAP_TREE_APPLY_INIT(apply, NULL, NULL, nb_tree2list, NULL)

  CMAP_TEST_ASSERT(check_sort(CMAP_T, &args, &apply, &nb_tree),
    "Check ge_first sort");
  CMAP_TEST_ASSERT(check_sort(CMAP_F, &args, &apply, &nb_tree),
    "Check not ge_first sort");

  /********** Check mem */
  CMAP_MEM_STATE * mem_state = cmap_mem_state();
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_chunk_ == 1);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block_ == SIZE + 3);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block_free_ == 1);
#ifdef DEBUG
  printf("Alloc size = %d\n", mem_state -> size_alloc_);
#endif
  CMAP_TEST_ASSERT_NOMSG(mem_state -> size_alloc_ ==
    SIZE * (sizeof(NB) + sizeof(int)));

  /********** Free mem */
  CMAP_TREE_APPLY_INIT(apply, NULL, NULL, NULL, nb_delete)
  CMAP_TREE_APPLYFN(nb, &nb_tree, apply, CMAP_T, mem);
  CMAP_TEST_ASSERT_NOMSG(nb_tree == NULL);

  mem -> free(args.list_);
  args.list_ = NULL;

  /********** Check mem */
  mem_state = cmap_mem_state();
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_chunk_ == 1);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block_ == 2);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block_free_ == 1);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> size_alloc_ == 0);

  return EXIT_SUCCESS;
}
