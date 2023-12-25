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
} APPLY_DATA;

static void apply(CMAP_LIFECYCLE ** lc, void * data)
{
  APPLY_DATA * data_ = (APPLY_DATA *)data;
  printf("data_ -> ptr = [%p]\n", data_ -> ptr);
  CMAP_TEST_ASSERT_NOMSG(*lc == data_ -> ptr);
  data_ -> ptr = (CMAP_LIFECYCLE *)(((void *)data_ -> ptr) + 1);
}

int main(int argc, char * argv[])
{
  CMAP_STACK_LC * stack = cmap_stack_lc_public.create(0);

  CMAP_CALL_ARGS(stack, push, (CMAP_LIFECYCLE *)3);
  CMAP_CALL_ARGS(stack, push, (CMAP_LIFECYCLE *)4);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(stack, size) == 2);

  CMAP_CALL_ARGS(stack, unshift, (CMAP_LIFECYCLE *)2);
  CMAP_CALL_ARGS(stack, unshift, (CMAP_LIFECYCLE *)1);
  CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(stack, size) == 4);

  APPLY_DATA data = { (CMAP_LIFECYCLE *)1 };
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

  return EXIT_SUCCESS;
}
