/*******************************************************************************
@TEST_DESC@ Push/pop/... on slist
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "test-assert.h"
#include "cmap-slist.h"
#include "cmap-mem.h"

typedef struct
{
  CMAP_LIFECYCLE * ptr;
  int inc;
} APPLY_DATA;

static void apply(CMAP_LIFECYCLE ** lc, void * data)
{
  APPLY_DATA * data_ = data;
  printf("lc = [%p]\n", *lc);
  printf("data_ -> ptr = [%p]\n", data_ -> ptr);
  CMAP_TEST_ASSERT_NOMSG(*lc == data_ -> ptr);
  data_ -> ptr = (CMAP_LIFECYCLE *)(((void *)data_ -> ptr) + data_ -> inc);
}

static void test_push_pop_unshift_shift()
{
  CMAP_SLIST_LC * list = cmap_slist_lc_create(0);

  CMAP_LIFECYCLE * lc;
  lc = (CMAP_LIFECYCLE *)3; cmap_slist_lc_push(list, &lc);
  lc = (CMAP_LIFECYCLE *)4; cmap_slist_lc_push(list, &lc);
  CMAP_TEST_ASSERT_NOMSG(cmap_slist_lc_size(list) == 2);

  lc = (CMAP_LIFECYCLE *)2; cmap_slist_lc_unshift(list, &lc);
  lc = (CMAP_LIFECYCLE *)1; cmap_slist_lc_unshift(list, &lc);
  CMAP_TEST_ASSERT_NOMSG(cmap_slist_lc_size(list) == 4);

  APPLY_DATA data = { (CMAP_LIFECYCLE *)1, 1 };
  cmap_slist_lc_apply(list, apply, &data);

  lc = NULL; for(int i = 0; i < 1024; i++) cmap_slist_lc_unshift(list, &lc);
  for(int i = 0; i < 1024; i++) cmap_slist_lc_pop(list);
  CMAP_TEST_ASSERT_NOMSG(cmap_slist_lc_size(list) == 4);

  for(int i = 0; i < 5; i++) cmap_slist_lc_shift(list);
  CMAP_TEST_ASSERT_NOMSG(cmap_slist_lc_size(list) == 0);

  printf("Allocated memory = [%d]\n", cmap_mem_state() -> size_alloc);
  CMAP_TEST_ASSERT_NOMSG(cmap_mem_state() -> size_alloc != 0);
  cmap_slist_lc_delete(list);
  CMAP_TEST_ASSERT_NOMSG(cmap_mem_state() -> size_alloc == 0);
}

static void test_add_rm()
{
  CMAP_SLIST_LC * list = cmap_slist_lc_create(3);

  CMAP_LIFECYCLE * lc;
  lc = (CMAP_LIFECYCLE *)3; cmap_slist_lc_add(list, 0, &lc); // 3
  lc = (CMAP_LIFECYCLE *)1; cmap_slist_lc_add(list, 0, &lc); // 13
  lc = (CMAP_LIFECYCLE *)9; cmap_slist_lc_add(list, 2, &lc); // 139
  lc = (CMAP_LIFECYCLE *)7; cmap_slist_lc_add(list, 2, &lc); // 1379
  lc = (CMAP_LIFECYCLE *)5; cmap_slist_lc_add(list, 2, &lc); // 13579
  lc = (CMAP_LIFECYCLE *)2; cmap_slist_lc_add(list, 1, &lc); // 123579
  lc = (CMAP_LIFECYCLE *)4; cmap_slist_lc_add(list, 3, &lc); // 1234579
  lc = (CMAP_LIFECYCLE *)8; cmap_slist_lc_add(list, 6, &lc); // 12345789
  lc = (CMAP_LIFECYCLE *)6; cmap_slist_lc_add(list, 5, &lc); // 123456789
  APPLY_DATA data = { (CMAP_LIFECYCLE *)1, 1 };
  cmap_slist_lc_apply(list, apply, &data);

  cmap_slist_lc_rm(list, 3); // 12356789
  cmap_slist_lc_rm(list, 4); // 1235789
  cmap_slist_lc_rm(list, 5); // 123579
  cmap_slist_lc_rm(list, 1); // 13579
  APPLY_DATA data2 = { (CMAP_LIFECYCLE *)1, 2 };
  cmap_slist_lc_apply(list, apply, &data2);
  cmap_slist_lc_rm(list, 2); // 1379
  cmap_slist_lc_rm(list, 3); // 137
  cmap_slist_lc_rm(list, 1); // 17
  APPLY_DATA data6 = { (CMAP_LIFECYCLE *)1, 6 };
  cmap_slist_lc_apply(list, apply, &data6);
  cmap_slist_lc_rm(list, 1); // 1
  cmap_slist_lc_rm(list, 0);
  CMAP_TEST_ASSERT_NOMSG(cmap_slist_lc_size(list) == 0);

  cmap_slist_lc_delete(list);
  CMAP_TEST_ASSERT_NOMSG(cmap_mem_state() -> size_alloc == 0);
}

int main(int argc, char * argv[])
{
  test_push_pop_unshift_shift();
  test_add_rm();
  return EXIT_SUCCESS;
}
