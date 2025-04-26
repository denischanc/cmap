/*******************************************************************************
@TEST_DESC@ Check memory with stree
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "test-assert.h"
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-stree.h"
#include "cmap-log.h"
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

#define SIZE 10
#define NB_MAX (1 << 16)
//#define DEBUG

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_STREE_NODE node;
  int nb;
} NB;

static int nb_eval(void * node, void * data)
{
  NB * node1 = (NB *)node, * node2 = (NB *)data;
  return (node1 -> nb - node2 -> nb);
}

static const char * nb_log(void * node)
{
  static char buffer[11];
  snprintf(buffer, sizeof(buffer), "%d", ((NB *)node) -> nb);
  return buffer;
}

CMAP_STREE_RUNNER(nb, nb_eval, nb_log, CMAP_F, CMAP_F);

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int i;
  int * list;
} STREE2LIST_ARGS;

static void nb_stree2list(void * node, char is_eq, void * data)
{
  STREE2LIST_ARGS * args = (STREE2LIST_ARGS *)data;
  int nb = ((NB *)node) -> nb;
  args -> list[args -> i++] = nb;

#ifdef DEBUG
  printf("%d ", nb);
#endif
}

static void nb_delete(void * node, char is_eq, void * data)
{
  CMAP_MEM * mem = (CMAP_MEM *)data;
  mem -> free(node);
}

/*******************************************************************************
*******************************************************************************/

static char check_sort(char gt_first, STREE2LIST_ARGS * args,
  CMAP_STREE_APPLY * apply, NB * stree)
{
  /********** Fill list */
  int i;
  for(i = 0; i < SIZE; i++)
  {
    args -> list[i] = gt_first ? i : SIZE - i - 1;
  }

  args -> i = 0;
  CMAP_STREE_APPLYFN(nb, stree, *apply, gt_first, CMAP_T, args);
#ifdef DEBUG
  printf("\n");
#endif

  /********** Check order */
  int prev = gt_first ? NB_MAX : -1, cur;
  for(i = 0; i < SIZE; i++)
  {
    cur = args -> list[i];
    if(gt_first)
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

CMAP_STREE_APPLY(stree2list_apply, NULL, nb_stree2list, NULL);

int main(int argc, char * argv[])
{
  CMAP_MEM * mem = cmap_mem_public.instance(0);
  NB * nb_stree = NULL, * tmp;

  /********** Fill stree */
  int i;
  for(i = 0; i < SIZE; i++)
  {
    tmp = (NB *)mem -> alloc(sizeof(NB));
    tmp -> nb = (random() % NB_MAX);

    CMAP_STREE_ADDFN(nb, &nb_stree, tmp, tmp);
  }
  cmap_kernel_public.dft_cfg() -> log_lvl = CMAP_LOG_INFO;
  CMAP_STREE_LOGFN(nb, CMAP_LOG_INFO, nb_stree);

  /********** Check stree */
  STREE2LIST_ARGS args;
  args.list = (int *)mem -> alloc(SIZE * sizeof(int));

  CMAP_TEST_ASSERT(check_sort(CMAP_T, &args, &stree2list_apply, nb_stree),
    "Check gt_first sort");
  CMAP_TEST_ASSERT(check_sort(CMAP_F, &args, &stree2list_apply, nb_stree),
    "Check not gt_first sort");

  /********** Check mem */
  CMAP_MEM_STATE * mem_state = cmap_mem_public.state();
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_chunk == 1);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block == SIZE + 3);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block_free == 1);
#ifdef DEBUG
  printf("Alloc size = %d\n", mem_state -> size_alloc);
#endif
  CMAP_TEST_ASSERT_NOMSG(mem_state -> size_alloc ==
    SIZE * (sizeof(NB) + sizeof(int)));

  /********** Free mem */
  CMAP_STREE_QUICKAPPLYFN(nb, nb_stree, nb_delete, mem);
  nb_stree = NULL;

  mem -> free(args.list);
  args.list = NULL;

  /********** Check mem */
  mem_state = cmap_mem_public.state();
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_chunk == 1);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block == 2);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> nb_block_free == 1);
  CMAP_TEST_ASSERT_NOMSG(mem_state -> size_alloc == 0);

  return EXIT_SUCCESS;
}
