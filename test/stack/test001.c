/*******************************************************************************
@TEST_DESC@ Push/pop/... on stack
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "test-assert.h"
#include "cmap-stack.h"
#include "cmap.h"
#include "cmap-mem.h"

typedef struct
{
  CMAP_LIFECYCLE * ptr;
  int inc;
} APPLY_DATA;

static void apply(CMAP_LIFECYCLE * lc, void * data)
{
  APPLY_DATA * data_ = (APPLY_DATA *)data;
  printf("lc = [%p]\n", lc);
  printf("data_ -> ptr = [%p]\n", data_ -> ptr);
  CMAP_TEST_ASSERT_NOMSG(lc == data_ -> ptr);
  data_ -> ptr = (CMAP_LIFECYCLE *)(((void *)data_ -> ptr) + data_ -> inc);
}

static void test_push_pop_unshift_shift()
{
  CMAP_STACK_LC * stack = cmap_stack_lc_public.create(0);

  CMAP_CALL_ARGS(stack, push, (CMAP_LIFECYCLE *)3);
  CMAP_CALL_ARGS(stack, push, (CMAP_LIFECYCLE *)4);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(stack, size) == 2);

  CMAP_CALL_ARGS(stack, unshift, (CMAP_LIFECYCLE *)2);
  CMAP_CALL_ARGS(stack, unshift, (CMAP_LIFECYCLE *)1);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(stack, size) == 4);

  APPLY_DATA data = { (CMAP_LIFECYCLE *)1, 1 };
  CMAP_CALL_ARGS(stack, apply, apply, &data);

  for(int i = 0; i < 1024; i++) CMAP_CALL_ARGS(stack, unshift, NULL);
  for(int i = 0; i < 1024; i++) CMAP_CALL(stack, pop);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(stack, size) == 4);

  for(int i = 0; i < 5; i++) CMAP_CALL(stack, shift);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(stack, size) == 0);

  printf("Allocated memory = [%d]\n", cmap_mem_public.state() -> size_alloc);
  CMAP_TEST_ASSERT_NOMSG(cmap_mem_public.state() -> size_alloc != 0);
  CMAP_CALL(stack, delete);
  CMAP_TEST_ASSERT_NOMSG(cmap_mem_public.state() -> size_alloc == 0);
}

static void test_add_rm()
{
  CMAP_STACK_LC * stack = cmap_stack_lc_public.create(3);

  CMAP_CALL_ARGS(stack, add, 0, (CMAP_LIFECYCLE *)3); // 3
  CMAP_CALL_ARGS(stack, add, 0, (CMAP_LIFECYCLE *)1); // 13
  CMAP_CALL_ARGS(stack, add, 2, (CMAP_LIFECYCLE *)9); // 139
  CMAP_CALL_ARGS(stack, add, 2, (CMAP_LIFECYCLE *)7); // 1379
  CMAP_CALL_ARGS(stack, add, 2, (CMAP_LIFECYCLE *)5); // 13579
  CMAP_CALL_ARGS(stack, add, 1, (CMAP_LIFECYCLE *)2); // 123579
  CMAP_CALL_ARGS(stack, add, 3, (CMAP_LIFECYCLE *)4); // 1234579
  CMAP_CALL_ARGS(stack, add, 6, (CMAP_LIFECYCLE *)8); // 12345789
  CMAP_CALL_ARGS(stack, add, 5, (CMAP_LIFECYCLE *)6); // 123456789
  APPLY_DATA data = { (CMAP_LIFECYCLE *)1, 1 };
  CMAP_CALL_ARGS(stack, apply, apply, &data);

  CMAP_CALL_ARGS(stack, rm, 3); // 12356789
  CMAP_CALL_ARGS(stack, rm, 4); // 1235789
  CMAP_CALL_ARGS(stack, rm, 5); // 123579
  CMAP_CALL_ARGS(stack, rm, 1); // 13579
  APPLY_DATA data2 = { (CMAP_LIFECYCLE *)1, 2 };
  CMAP_CALL_ARGS(stack, apply, apply, &data2);
  CMAP_CALL_ARGS(stack, rm, 2); // 1379
  CMAP_CALL_ARGS(stack, rm, 3); // 137
  CMAP_CALL_ARGS(stack, rm, 1); // 17
  APPLY_DATA data6 = { (CMAP_LIFECYCLE *)1, 6 };
  CMAP_CALL_ARGS(stack, apply, apply, &data6);
  CMAP_CALL_ARGS(stack, rm, 1); // 1
  CMAP_CALL_ARGS(stack, rm, 0);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(stack, size) == 0);

  CMAP_CALL(stack, delete);
  CMAP_TEST_ASSERT_NOMSG(cmap_mem_public.state() -> size_alloc == 0);
}

int main(int argc, char * argv[])
{
  test_push_pop_unshift_shift();
  test_add_rm();
  return EXIT_SUCCESS;
}
