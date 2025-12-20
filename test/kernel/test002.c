/*******************************************************************************
@TEST_DESC@ Add/rm maps in list
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "test-assert.h"
#include "cmap.h"
#include "cmap-list.h"

/*******************************************************************************
*******************************************************************************/

#define NB_MIN 3
#define NB_MAX 7

//#define DEBUG

/*******************************************************************************
*******************************************************************************/

static void display_list_ptr(CMAP_LIST * list)
{
#ifdef DEBUG
  int i, size = CMAP_CALL(list, size);
  for(i = 0; i < size; i++)
  {
    printf("%d : %p\n", i, CMAP_CALL_ARGS(list, get, i));
  }
#endif
}

static void test_list(int size, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_TEST_ASSERT_NOMSG(size >= NB_MIN);
  CMAP_TEST_ASSERT_NOMSG(size <= NB_MAX);

  CMAP_LIST * list = cmap_list(size, proc_ctx);

  static CMAP_MAP * elmts[NB_MAX];
  int i = 0;
  for(; i < size; i++) elmts[i] = cmap_map(proc_ctx);

  /********** push/unshift */
  for(i = 0; i < size; i++) CMAP_CALL_ARGS(list, push, elmts[i]);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == size);
  for(i = 0; i < size; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, shift) == elmts[i]);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == 0);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, shift) == NULL);

  /********** shift/pop */
  for(i = 0; i < size; i++) CMAP_CALL_ARGS(list, unshift, elmts[i]);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == size);
  for(i = 0; i < size; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, pop) == elmts[i]);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == 0);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, pop) == NULL);

  /********** add/rm */
  for(i = 0; i < size; i++) CMAP_CALL_ARGS(list, add, i, elmts[i]);
  display_list_ptr(list);
  for(i = 0; i < size; i++)
  {
    CMAP_CALL_ARGS(list, add, 1, elmts[i]);
    display_list_ptr(list);
  }
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == (2 * size));
  for(i = 1; i <= size; i++) CMAP_TEST_ASSERT_NOMSG(
    CMAP_CALL_ARGS(list, get, i) == elmts[size - i]);

  for(i = 0; i < size; i++) CMAP_CALL_ARGS(list, rm, 2);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == size);
  for(i = 0; i < size; i++)
  {
    if(i == 1) CMAP_TEST_ASSERT_NOMSG(
      CMAP_CALL_ARGS(list, get, i) == elmts[size - 1]);
    else CMAP_TEST_ASSERT_NOMSG(CMAP_CALL_ARGS(list, get, i) == elmts[i]);
  }

  CMAP_CALL_ARGS(list, set, 1, elmts[1]);
  for(i = 0; i < size; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL_ARGS(list, get, i) == elmts[i]);
  for(i = 0; i < size; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, shift) == elmts[i]);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == 0);

  /********** push/pop until max size */
  for(i = 0; i < (3 * size); i++)
    CMAP_CALL_ARGS(list, push, elmts[i % size]);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == (3 * size));
  for(i = ((3 * size) - 1); i >= 0; i--)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, pop) == elmts[i % size]);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == 0);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, pop) == NULL);
}

static void test(CMAP_PROC_CTX * proc_ctx)
{
  test_list(3, proc_ctx);
  test_list(4, proc_ctx);
  test_list(5, proc_ctx);
  test_list(7, proc_ctx);
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(argc, argv);
  cmap_env_main(cmap_env(), test);
  return cmap_main();
}
