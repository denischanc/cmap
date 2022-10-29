/*******************************************************************************
@TEST_DESC@ Alloc/dealloc with 1024 memory
*******************************************************************************/

#include "cmap-mem.h"
#include <cmap/cmap.h>

#include "test-assert.h"
#include <stdlib.h>

#define ASSERT_NB_FREE(nb) \
  CMAP_TEST_ASSERT(cmap_mem_state() -> nb_block_free == nb, \
    "Check free block number")

#define ASSERT_NB_CHUNK(nb) \
  CMAP_TEST_ASSERT(cmap_mem_state() -> nb_chunk == nb, \
    "Check chunk number")

int main(int argc, char * argv[])
{
  CMAP_MEM * mem = cmap_mem_public.init(1024);
  CMAP_TEST_ASSERT_PTR(mem);

  ASSERT_NB_CHUNK(0);

  void * ptr1 = mem -> alloc(100);
  CMAP_TEST_ASSERT_PTR(ptr1);
  ASSERT_NB_CHUNK(1);

  void * ptr2 = mem -> alloc(300);
  CMAP_TEST_ASSERT_PTR(ptr2);
  ASSERT_NB_CHUNK(1);

  void * ptr3 = mem -> alloc(10000);
  CMAP_TEST_ASSERT_PTR(ptr3);
  ASSERT_NB_CHUNK(2);

  mem -> free(ptr1);

  void * ptr4 = mem -> alloc(200);
  CMAP_TEST_ASSERT_PTR(ptr4);
  CMAP_TEST_ASSERT(ptr4 > ptr2, "Check ptr4");

  void * ptr5 = mem -> alloc(100);
  CMAP_TEST_ASSERT_PTR(ptr5);
  CMAP_TEST_ASSERT(ptr5 == ptr1, "Check ptr5");

  ASSERT_NB_FREE(1);
  mem -> free(ptr2);
  ASSERT_NB_FREE(2);
  mem -> free(ptr3);
  ASSERT_NB_FREE(3);
  mem -> free(ptr4);
  ASSERT_NB_FREE(2);
  mem -> free(ptr5);
  ASSERT_NB_FREE(2);
  ASSERT_NB_CHUNK(2);

  return EXIT_SUCCESS;
}
