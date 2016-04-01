/*******************************************************************************
@TEST_DESC@ Add/rm maps in list
*******************************************************************************/

#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-fw.h"

#include "test-assert.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
*******************************************************************************/

#define NB_ELMTS_1 3

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

int main(int argc, char * argv[])
{
  cmap_kernel_create(NULL);

  CMAP_LIST * list = _L_(3);

  CMAP_MAP elmts[NB_ELMTS_1] = {0};
  int i = 0;

  for(i = 0; i < NB_ELMTS_1; i++) CMAP_CALL_ARGS(list, push, elmts + i);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == NB_ELMTS_1);
  for(i = 0; i < NB_ELMTS_1; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, unshift) == elmts + i);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == 0);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, unshift) == NULL);

  for(i = 0; i < NB_ELMTS_1; i++) CMAP_CALL_ARGS(list, shift, elmts + i);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == NB_ELMTS_1);
  for(i = 0; i < NB_ELMTS_1; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, pop) == elmts + i);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == 0);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, pop) == NULL);

  for(i = 0; i < NB_ELMTS_1; i++) CMAP_CALL_ARGS(list, add, i, elmts + i);
  display_list_ptr(list);
  for(i = 0; i < NB_ELMTS_1; i++)
  {
    CMAP_CALL_ARGS(list, add, 1, elmts + i);
    display_list_ptr(list);
  }
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == (2 * NB_ELMTS_1));
  for(i = 1; i <= NB_ELMTS_1; i++) CMAP_TEST_ASSERT_NOMSG(
    CMAP_CALL_ARGS(list, get, i) == elmts + (NB_ELMTS_1 - i));

  for(i = 0; i < NB_ELMTS_1; i++) CMAP_CALL_ARGS(list, rm, 2);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == NB_ELMTS_1);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL_ARGS(list, get, 0) == elmts);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL_ARGS(list, get, 1) == elmts + 2);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL_ARGS(list, get, 2) == elmts + 2);

  CMAP_CALL_ARGS(list, set, 1, elmts + 1);
  for(i = 0; i < NB_ELMTS_1; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL_ARGS(list, get, i) == elmts + i);
  for(i = 0; i < NB_ELMTS_1; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, unshift) == elmts + i);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, size) == 0);

  CMAP_CALL(((CMAP_MAP *)list), delete);

  return cmap_kernel() -> main(argc, argv);
}
